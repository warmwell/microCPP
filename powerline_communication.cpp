#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES  // 需在#include <cmath>之前定义
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// 模拟参数定义
const int SAMPLE_RATE = 44100;    // 采样率
const double CARRIER_FREQ = 1000; // 载波频率1kHz
const double BIT_RATE = 100;      // 比特率100bps
const int SAMPLES_PER_BIT = SAMPLE_RATE / BIT_RATE; // 每比特的采样数
// const double M_PI = acos(-1.0);

// 生成随机数字信号
vector<bool> generateDigitalSignal(int length) {
    vector<bool> signal;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        signal.push_back(rand() % 2 == 0);
    }
    return signal;
}

// 振幅键控(ASK)调制: 0表示低振幅,1表示高振幅
vector<double> modulateASK(const vector<bool>& digitalSignal) {
    vector<double> modulatedSignal;
    
    for (bool bit : digitalSignal) {
        // 为每个比特生成多个采样点
        for (int i = 0; i < SAMPLES_PER_BIT; ++i) {
            double t = static_cast<double>(i) / SAMPLE_RATE;
            // 载波信号: 振幅根据比特值变化
            double amplitude = bit ? 1.0 : 0.2; // 1用高振幅,0用低振幅
            double sample = amplitude * sin(2 * M_PI * CARRIER_FREQ * t);
            modulatedSignal.push_back(sample);
        }
    }
    
    return modulatedSignal;
}

// 模拟电力线传输(添加噪声)
vector<double> transmitSignal(const vector<double>& signal) {
    vector<double> receivedSignal;
    srand(time(0) + 1); // 不同的随机种子
    
    for (double sample : signal) {
        // 添加高斯噪声模拟电力线干扰
        double noise = (rand() % 200 - 100) / 500.0; // 范围: -0.2到0.2
        receivedSignal.push_back(sample + noise);
    }
    
    return receivedSignal;
}

// ASK解调: 通过检测振幅恢复数字信号
vector<bool> demodulateASK(const vector<double>& receivedSignal) {
    vector<bool> digitalSignal;
    
    // 按每比特的采样数分组处理
    for (int i = 0; i < receivedSignal.size(); i += SAMPLES_PER_BIT) {
        double amplitudeSum = 0;
        int count = 0;
        
        // 计算该比特周期内的平均振幅
        for (int j = 0; j < SAMPLES_PER_BIT && (i + j) < receivedSignal.size(); ++j) {
            amplitudeSum += abs(receivedSignal[i + j]);
            count++;
        }
        
        double avgAmplitude = amplitudeSum / count;
        // 根据平均振幅判断是0还是1
        digitalSignal.push_back(avgAmplitude > 0.5); // 阈值判断
    }
    
    return digitalSignal;
}

// 打印二进制信号
void printSignal(const vector<bool>& signal, const string& label) {
    cout << label << ": ";
    for (bool bit : signal) {
        cout << (bit ? "1" : "0");
    }
    cout << endl;
}

int main() {
    // 1. 生成原始数字信号
    vector<bool> originalSignal = generateDigitalSignal(16); // 16位随机信号
    printSignal(originalSignal, "原始信号");
    
    // 2. 调制到载波上
    vector<double> modulated = modulateASK(originalSignal);
    cout << "已将信号调制到 " << CARRIER_FREQ << "Hz 载波上" << endl;
    
    // 3. 模拟电力线传输
    vector<double> received = transmitSignal(modulated);
    cout << "信号通过电力线传输(已添加模拟噪声)" << endl;
    
    // 4. 解调恢复信号
    vector<bool> recoveredSignal = demodulateASK(received);
    printSignal(recoveredSignal, "解调后信号");
    
    // 5. 验证结果
    bool success = true;
    for (int i = 0; i < originalSignal.size(); ++i) {
        if (originalSignal[i] != recoveredSignal[i]) {
            success = false;
            break;
        }
    }
    
    if (success) {
        cout << "通信成功: 原始信号与解调信号一致" << endl;
    } else {
        cout << "通信存在误差: 原始信号与解调信号不一致" << endl;
    }
    
    return 0;
}