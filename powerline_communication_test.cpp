#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// 模拟参数定义
const int SAMPLE_RATE = 44100;
const double CARRIER_FREQ = 1000;
const double BIT_RATE = 100;
const int SAMPLES_PER_BIT = SAMPLE_RATE / BIT_RATE;

// 原有的函数声明和实现
vector<bool> generateDigitalSignal(int length) {
    vector<bool> signal;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        signal.push_back(rand() % 2 == 0);
    }
    return signal;
}

// 生成特定模式的测试信号
vector<bool> generateTestSignal(const string& pattern) {
    vector<bool> signal;
    for (char c : pattern) {
        if (c == '1') signal.push_back(true);
        else if (c == '0') signal.push_back(false);
    }
    return signal;
}

vector<double> modulateASK(const vector<bool>& digitalSignal) {
    vector<double> modulatedSignal;
    
    for (bool bit : digitalSignal) {
        for (int i = 0; i < SAMPLES_PER_BIT; ++i) {
            double t = static_cast<double>(i) / SAMPLE_RATE;
            double amplitude = bit ? 1.0 : 0.2;
            double sample = amplitude * sin(2 * M_PI * CARRIER_FREQ * t);
            modulatedSignal.push_back(sample);
        }
    }
    
    return modulatedSignal;
}

vector<double> transmitSignal(const vector<double>& signal, double noiseLevel = 0.4) {
    vector<double> receivedSignal;
    
    for (double sample : signal) {
        double noise = (rand() % 200 - 100) / 500.0 * noiseLevel;
        receivedSignal.push_back(sample + noise);
    }
    
    return receivedSignal;
}

vector<bool> demodulateASK(const vector<double>& receivedSignal) {
    vector<bool> digitalSignal;
    
    for (int i = 0; i < receivedSignal.size(); i += SAMPLES_PER_BIT) {
        double amplitudeSum = 0;
        int count = 0;
        
        for (int j = 0; j < SAMPLES_PER_BIT && (i + j) < receivedSignal.size(); ++j) {
            amplitudeSum += abs(receivedSignal[i + j]);
            count++;
        }
        
        double avgAmplitude = amplitudeSum / count;
        digitalSignal.push_back(avgAmplitude > 0.5);
    }
    
    return digitalSignal;
}

void printSignal(const vector<bool>& signal, const string& label) {
    cout << label << ": ";
    for (bool bit : signal) {
        cout << (bit ? "1" : "0");
    }
    cout << endl;
}

// ================ 测试函数 ================

// 测试数据结构
struct TestCase {
    string name;
    string pattern;
    double expectedSuccessRate;
    string description;
};

// 预定义测试用例
vector<TestCase> getTestCases() {
    return {
        {"全零信号", "0000000000000000", 0.95, "测试连续低振幅信号的传输"},
        {"全一信号", "1111111111111111", 0.95, "测试连续高振幅信号的传输"},
        {"交替信号", "1010101010101010", 0.90, "测试高低振幅快速切换的信号"},
        {"短脉冲", "0001000", 0.85, "测试短脉冲信号"},
        {"长脉冲", "0001111111000", 0.90, "测试长脉冲信号"},
        {"随机短信号", "10110", 0.80, "测试短随机信号"},
        {"复杂模式1", "110011001100", 0.85, "测试复杂重复模式"},
        {"复杂模式2", "101110001011", 0.80, "测试不规则模式"},
        {"边界测试1", "1", 0.70, "测试单比特信号"},
        {"边界测试2", "10", 0.75, "测试两比特信号"}
    };
}

// 单个测试用例执行函数
bool runSingleTest(const TestCase& testCase, double noiseLevel = 0.4, bool verbose = false) {
    if (verbose) {
        cout << "\n=== " << testCase.name << " ===" << endl;
        cout << "描述: " << testCase.description << endl;
    }
    
    vector<bool> original = generateTestSignal(testCase.pattern);
    vector<double> modulated = modulateASK(original);
    vector<double> received = transmitSignal(modulated, noiseLevel);
    vector<bool> recovered = demodulateASK(received);
    
    if (verbose) {
        printSignal(original, "原始信号");
        printSignal(recovered, "恢复信号");
    }
    
    // 检查信号长度
    if (original.size() != recovered.size()) {
        if (verbose) cout << "错误: 信号长度不匹配!" << endl;
        return false;
    }
    
    // 计算匹配率
    int matches = 0;
    for (int i = 0; i < original.size(); ++i) {
        if (original[i] == recovered[i]) matches++;
    }
    
    double successRate = static_cast<double>(matches) / original.size();
    bool passed = successRate >= testCase.expectedSuccessRate;
    
    if (verbose) {
        cout << "匹配率: " << fixed << setprecision(2) << successRate * 100 << "%" << endl;
        cout << "期望率: " << testCase.expectedSuccessRate * 100 << "%" << endl;
        cout << "测试结果: " << (passed ? "通过" : "失败") << endl;
    }
    
    return passed;
}

// 噪声级别测试
void noiseResistanceTest() {
    cout << "\n======== 噪声抗性测试 ========" << endl;
    
    string testPattern = "1010101010101010";
    vector<double> noiseLevels = {0.1, 0.2, 0.4, 0.6, 0.8, 1.0};
    
    cout << "测试信号: " << testPattern << endl;
    cout << "噪声级别\t成功率" << endl;
    cout << "----------------------" << endl;
    
    for (double noise : noiseLevels) {
        int successCount = 0;
        int totalTests = 50; // 多次测试求平均
        
        for (int i = 0; i < totalTests; ++i) {
            srand(time(0) + i); // 不同的随机种子
            
            vector<bool> original = generateTestSignal(testPattern);
            vector<double> modulated = modulateASK(original);
            vector<double> received = transmitSignal(modulated, noise);
            vector<bool> recovered = demodulateASK(received);
            
            if (original == recovered) {
                successCount++;
            }
        }
        
        double successRate = static_cast<double>(successCount) / totalTests;
        cout << fixed << setprecision(1) << noise << "\t\t" 
             << setprecision(1) << successRate * 100 << "%" << endl;
    }
}

// 性能测试
void performanceTest() {
    cout << "\n======== 性能测试 ========" << endl;
    
    vector<int> signalLengths = {8, 16, 32, 64, 128, 256};
    
    cout << "信号长度\t处理时间(相对)" << endl;
    cout << "------------------------" << endl;
    
    clock_t baseTime = 0;
    
    for (int length : signalLengths) {
        vector<bool> signal = generateDigitalSignal(length);
        
        clock_t start = clock();
        
        vector<double> modulated = modulateASK(signal);
        vector<double> received = transmitSignal(modulated);
        vector<bool> recovered = demodulateASK(received);
        
        clock_t end = clock();
        clock_t duration = end - start;
        
        if (baseTime == 0) baseTime = duration;
        
        double relative = static_cast<double>(duration) / baseTime;
        
        cout << length << "\t\t" << fixed << setprecision(2) << relative << endl;
    }
}

// 边界条件测试
void boundaryTest() {
    cout << "\n======== 边界条件测试 ========" << endl;
    
    // 测试空信号
    cout << "测试空信号..." << endl;
    vector<bool> empty;
    vector<double> emptyMod = modulateASK(empty);
    vector<bool> emptyRec = demodulateASK(emptyMod);
    cout << "空信号测试: " << (emptyRec.empty() ? "通过" : "失败") << endl;
    
    // 测试极长信号
    cout << "测试极长信号(1000位)..." << endl;
    vector<bool> longSignal = generateDigitalSignal(1000);
    vector<double> longMod = modulateASK(longSignal);
    vector<double> longRec = transmitSignal(longMod, 0.2); // 低噪声
    vector<bool> longDemod = demodulateASK(longRec);
    
    int matches = 0;
    for (int i = 0; i < longSignal.size(); ++i) {
        if (longSignal[i] == longDemod[i]) matches++;
    }
    double longSuccessRate = static_cast<double>(matches) / longSignal.size();
    cout << "极长信号成功率: " << fixed << setprecision(1) << longSuccessRate * 100 << "%" << endl;
}

// 主测试函数
void runAllTests() {
    cout << "======== 电力线通信仿真测试套件 ========" << endl;
    
    // 基本功能测试
    cout << "\n1. 基本功能测试" << endl;
    cout << "=================" << endl;
    
    vector<TestCase> testCases = getTestCases();
    int passed = 0;
    int total = testCases.size();
    
    for (const auto& test : testCases) {
        if (runSingleTest(test, 0.4, false)) {
            cout << "✓ " << test.name << " - 通过" << endl;
            passed++;
        } else {
            cout << "✗ " << test.name << " - 失败" << endl;
        }
    }
    
    cout << "\n基本测试结果: " << passed << "/" << total << " 通过" << endl;
    
    // 详细测试一个案例
    cout << "\n2. 详细测试示例" << endl;
    cout << "=================" << endl;
    runSingleTest(testCases[2], 0.4, true); // 交替信号详细测试
    
    // 噪声抗性测试
    noiseResistanceTest();
    
    // 性能测试
    performanceTest();
    
    // 边界条件测试
    boundaryTest();
    
    cout << "\n======== 测试完成 ========" << endl;
}

// 交互式测试函数
void interactiveTest() {
    cout << "\n======== 交互式测试 ========" << endl;
    cout << "请输入测试信号模式 (只包含0和1, 例如: 10110): ";
    
    string pattern;
    cin >> pattern;
    
    // 验证输入
    for (char c : pattern) {
        if (c != '0' && c != '1') {
            cout << "错误: 信号模式只能包含0和1!" << endl;
            return;
        }
    }
    
    cout << "请输入噪声级别 (0.0-1.0, 推荐0.4): ";
    double noise;
    cin >> noise;
    
    if (noise < 0.0 || noise > 1.0) {
        cout << "错误: 噪声级别应在0.0-1.0之间!" << endl;
        return;
    }
    
    TestCase userTest = {"用户自定义", pattern, 0.5, "用户输入的测试信号"};
    runSingleTest(userTest, noise, true);
}

int main() {
    // 运行所有自动测试
    runAllTests();
    
    // 提供交互式测试选项
    cout << "\n是否进行交互式测试? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        interactiveTest();
    }
    
    return 0;
}