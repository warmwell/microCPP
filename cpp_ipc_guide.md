# C++进程间通信(IPC)方式详解

进程间通信（Inter-Process Communication, IPC）是操作系统中不同进程之间进行数据交换和协调工作的机制。在C++中，有多种IPC方式可供选择，每种方式都有其特点和适用场景。

## 1. 匿名管道（Anonymous Pipes）

匿名管道是最简单的IPC方式，主要用于父子进程间的单向通信。

### 特点：
- 只能用于有亲缘关系的进程（父子进程）
- 单向通信（需要两个管道实现双向通信）
- 数据以字节流形式传输
- 简单高效

### 代码示例：
```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipefd[2];  // 管道文件描述符
    pid_t pid;
    
    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    pid = fork();  // 创建子进程
    
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {  // 子进程
        close(pipefd[1]);  // 关闭写端
        
        char buffer[100];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        buffer[bytes_read] = '\0';
        
        std::cout << "子进程接收到: " << buffer << std::endl;
        close(pipefd[0]);
    } else {  // 父进程
        close(pipefd[0]);  // 关闭读端
        
        const char* message = "Hello from parent!";
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]);
        
        wait(nullptr);  // 等待子进程结束
    }
    
    return 0;
}
```

## 2. 命名管道（Named Pipes/FIFO）

命名管道允许无亲缘关系的进程进行通信，通过文件系统中的特殊文件实现。

### 特点：
- 可用于任意进程间通信
- 双向通信
- 持久存在于文件系统中
- 支持多个读者和写者

### 代码示例：

**创建命名管道：**
```cpp
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

// 写进程
int main() {
    const char* fifo_path = "/tmp/my_fifo";
    
    // 创建命名管道
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }
    
    // 打开管道进行写操作
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    const char* message = "Hello from writer!";
    write(fd, message, strlen(message));
    
    close(fd);
    unlink(fifo_path);  // 删除管道文件
    
    return 0;
}
```

**读进程：**
```cpp
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char* fifo_path = "/tmp/my_fifo";
    
    // 打开管道进行读操作
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    
    std::cout << "读取到: " << buffer << std::endl;
    
    close(fd);
    return 0;
}
```

## 3. 消息队列（Message Queues）

消息队列提供了基于消息的异步通信机制。

### 特点：
- 异步通信
- 消息有类型和优先级
- 消息持久化（进程结束后仍存在）
- 支持多个发送者和接收者

### 代码示例（System V消息队列）：
```cpp
#include <iostream>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <cstring>

struct message {
    long msg_type;
    char msg_text[100];
};

// 发送者
int main() {
    key_t key = ftok("/tmp", 'A');
    int msgid = msgget(key, IPC_CREAT | 0666);
    
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }
    
    message msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello from sender!");
    
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        return 1;
    }
    
    std::cout << "消息已发送" << std::endl;
    return 0;
}

// 接收者（在另一个程序中）
int receiver_main() {
    key_t key = ftok("/tmp", 'A');
    int msgid = msgget(key, 0666);
    
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }
    
    message msg;
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }
    
    std::cout << "接收到消息: " << msg.msg_text << std::endl;
    
    // 删除消息队列
    msgctl(msgid, IPC_RMID, nullptr);
    
    return 0;
}
```

## 4. 共享内存（Shared Memory）

共享内存是最快的IPC方式，允许多个进程共享同一块内存区域。

### 特点：
- 最快的IPC方式
- 需要同步机制防止竞争条件
- 内存持久化
- 适合大量数据交换

### 代码示例：
```cpp
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <cstring>
#include <unistd.h>

// 写进程
int main() {
    key_t key = ftok("/tmp", 'B');
    int shmid = shmget(key, 1024, IPC_CREAT | 0666);
    
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
    
    // 附加共享内存
    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        perror("shmat");
        return 1;
    }
    
    // 写入数据
    strcpy(shared_memory, "Hello from shared memory!");
    
    std::cout << "数据已写入共享内存" << std::endl;
    
    // 分离共享内存
    shmdt(shared_memory);
    
    return 0;
}

// 读进程（在另一个程序中）
int reader_main() {
    key_t key = ftok("/tmp", 'B');
    int shmid = shmget(key, 1024, 0666);
    
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
    
    // 附加共享内存
    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        perror("shmat");
        return 1;
    }
    
    // 读取数据
    std::cout << "从共享内存读取: " << shared_memory << std::endl;
    
    // 分离和删除共享内存
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, nullptr);
    
    return 0;
}
```

## 5. 信号量（Semaphores）

信号量主要用于进程同步，控制对共享资源的访问。

### 特点：
- 主要用于同步，不传输数据
- 防止竞争条件
- 支持计数信号量和二进制信号量

### 代码示例：
```cpp
#include <iostream>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

class Semaphore {
private:
    int semid;
    
public:
    Semaphore(key_t key, int initial_value = 1) {
        semid = semget(key, 1, IPC_CREAT | 0666);
        if (semid == -1) {
            perror("semget");
            return;
        }
        
        semun sem_union;
        sem_union.val = initial_value;
        semctl(semid, 0, SETVAL, sem_union);
    }
    
    void wait() {  // P操作
        sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);
    }
    
    void signal() {  // V操作
        sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);
    }
    
    ~Semaphore() {
        semctl(semid, 0, IPC_RMID);
    }
};

int main() {
    key_t key = ftok("/tmp", 'C');
    Semaphore sem(key);
    
    std::cout << "等待信号量..." << std::endl;
    sem.wait();
    
    std::cout << "进入临界区" << std::endl;
    sleep(2);  // 模拟临界区操作
    std::cout << "离开临界区" << std::endl;
    
    sem.signal();
    
    return 0;
}
```

## 6. 套接字（Sockets）

套接字支持本地和网络通信，是最通用的IPC方式。

### 特点：
- 支持本地和网络通信
- 支持TCP和UDP协议
- 跨平台兼容性好
- 功能强大但相对复杂

### 代码示例（Unix域套接字）：

**服务器：**
```cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }
    
    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/socket_server");
    
    unlink("/tmp/socket_server");  // 删除可能存在的套接字文件
    
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }
    
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        return 1;
    }
    
    std::cout << "服务器等待连接..." << std::endl;
    
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd == -1) {
        perror("accept");
        return 1;
    }
    
    char buffer[100];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    
    std::cout << "接收到: " << buffer << std::endl;
    
    const char* response = "Hello from server!";
    write(client_fd, response, strlen(response));
    
    close(client_fd);
    close(server_fd);
    unlink("/tmp/socket_server");
    
    return 0;
}
```

**客户端：**
```cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

int main() {
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return 1;
    }
    
    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/socket_server");
    
    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }
    
    const char* message = "Hello from client!";
    write(client_fd, message, strlen(message));
    
    char buffer[100];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    buffer[bytes_read] = '\0';
    
    std::cout << "接收到回复: " << buffer << std::endl;
    
    close(client_fd);
    return 0;
}
```

## 7. 现代C++的IPC方案

### 使用Boost.Interprocess
```cpp
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

using namespace boost::interprocess;

int main() {
    try {
        // 创建共享内存对象
        shared_memory_object shm(create_only, "MySharedMemory", read_write);
        shm.truncate(1024);
        
        // 映射共享内存
        mapped_region region(shm, read_write);
        
        // 写入数据
        std::strcpy(static_cast<char*>(region.get_address()), "Hello Boost!");
        
        std::cout << "数据已写入共享内存" << std::endl;
        
    } catch (interprocess_exception& ex) {
        std::cout << "错误: " << ex.what() << std::endl;
        shared_memory_object::remove("MySharedMemory");
        return 1;
    }
    
    return 0;
}
```

## IPC方式选择指南

| IPC方式 | 性能 | 复杂度 | 适用场景 |
|---------|------|--------|----------|
| 匿名管道 | 中等 | 简单 | 父子进程简单通信 |
| 命名管道 | 中等 | 简单 | 本地进程间通信 |
| 消息队列 | 中等 | 中等 | 异步消息传递 |
| 共享内存 | 最高 | 复杂 | 大量数据交换 |
| 信号量 | - | 中等 | 进程同步 |
| 套接字 | 较低 | 复杂 | 网络通信或复杂本地通信 |

## 编译和运行建议

1. **编译命令**：
```bash
g++ -o program program.cpp -lrt -lpthread
```

2. **权限设置**：某些IPC方式需要适当的系统权限

3. **清理资源**：使用完毕后要及时清理IPC资源，避免系统资源泄露

4. **错误处理**：生产环境中要添加完善的错误处理机制

选择合适的IPC方式需要考虑性能要求、系统复杂度、维护成本等多个因素。对于简单的父子进程通信，推荐使用管道；对于高性能数据交换，推荐共享内存配合信号量；对于分布式系统，推荐使用套接字。