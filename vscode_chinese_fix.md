# VS Code 终端中文乱码解决方案

## 🔍 问题诊断

当前系统编码状态：
- ✅ 系统Locale: `en_US.UTF-8`
- ✅ 终端编码: UTF-8 支持
- ✅ 中文字符测试: 正常显示

如果VS Code终端仍显示中文乱码，问题可能出现在VS Code的配置上。

## 🛠️ 解决方案

### 方案1: VS Code 设置配置

1. **打开VS Code设置**
   - 按 `Ctrl+,` (Windows/Linux) 或 `Cmd+,` (macOS)
   - 或者 `File` → `Preferences` → `Settings`

2. **搜索并配置以下设置：**

```json
{
    "terminal.integrated.fontFamily": "Consolas, 'Courier New', monospace",
    "terminal.integrated.fontSize": 14,
    "terminal.integrated.localEchoLatencyThreshold": -1,
    "terminal.integrated.profiles.linux": {
        "bash": {
            "path": "bash",
            "env": {
                "LANG": "en_US.UTF-8",
                "LC_ALL": "en_US.UTF-8"
            }
        }
    },
    "terminal.integrated.defaultProfile.linux": "bash"
}
```

### 方案2: VS Code 用户设置 (settings.json)

1. **打开设置文件**
   - 按 `Ctrl+Shift+P` 打开命令面板
   - 输入 `Preferences: Open Settings (JSON)`
   - 添加以下配置：

```json
{
    "terminal.integrated.env.linux": {
        "LANG": "en_US.UTF-8",
        "LC_ALL": "en_US.UTF-8"
    },
    "terminal.integrated.fontFamily": "'DejaVu Sans Mono', 'Noto Sans CJK SC', monospace",
    "terminal.integrated.fontSize": 14
}
```

### 方案3: 工作区设置

在项目根目录创建 `.vscode/settings.json`：

```json
{
    "terminal.integrated.env.linux": {
        "LANG": "en_US.UTF-8",
        "LC_ALL": "en_US.UTF-8",
        "TERM": "xterm-256color"
    },
    "terminal.integrated.fontFamily": "monospace"
}
```

### 方案4: 终端启动命令

在VS Code终端中手动执行：

```bash
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export TERM=xterm-256color
```

### 方案5: Shell 配置文件

添加到 `~/.bashrc` 或 `~/.zshrc`：

```bash
# 确保UTF-8编码
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export TERM=xterm-256color

# 如果仍有问题，可以尝试
# export PYTHONIOENCODING=utf-8
```

## 🔧 字体配置

### 推荐的中文字体配置：

```json
{
    "terminal.integrated.fontFamily": "'Fira Code', 'Source Code Pro', 'DejaVu Sans Mono', 'Noto Sans CJK SC', 'SimHei', monospace"
}
```

### Windows 用户：
```json
{
    "terminal.integrated.fontFamily": "'Consolas', 'Microsoft YaHei', 'SimHei', monospace"
}
```

### macOS 用户：
```json
{
    "terminal.integrated.fontFamily": "'SF Mono', 'PingFang SC', 'Hiragino Sans GB', monospace"
}
```

## 🧪 测试方法

1. **重启VS Code终端**
   - 按 `Ctrl+Shift+\`` 打开终端
   - 点击终端右上角的垃圾桶图标关闭
   - 重新打开终端

2. **测试中文显示**
   ```bash
   echo "中文测试：你好世界！"
   ./build/merge_sorted_arrays
   ```

## 🔄 高级解决方案

### 如果上述方法都不起作用：

1. **检查VS Code版本**
   ```bash
   code --version
   ```

2. **重置VS Code终端**
   - `Ctrl+Shift+P` → `Terminal: Kill All Terminals`
   - `Ctrl+Shift+P` → `Developer: Reload Window`

3. **使用外部终端**
   ```json
   {
       "terminal.external.linuxExec": "gnome-terminal",
       "terminal.integrated.automationProfile.linux": null
   }
   ```

## 📝 常见问题

### Q: 为什么其他终端正常，VS Code不正常？
A: VS Code有自己的终端环境变量配置，可能覆盖了系统设置。

### Q: 设置后仍然乱码怎么办？
A: 尝试重启VS Code，或者使用 `Developer: Reload Window` 命令。

### Q: 只有特定程序输出乱码？
A: 检查程序的编码设置，确保使用UTF-8输出。

## ✅ 验证解决方案

配置完成后，运行以下命令验证：

```bash
# 1. 检查环境变量
echo $LANG $LC_ALL

# 2. 测试中文输出
echo "测试：你好世界！🌍"

# 3. 运行我们的程序
./build/merge_sorted_arrays

# 4. Python中文测试
python3 -c "print('Python中文：你好！')"
```

如果所有测试都正常显示中文，说明问题已解决！