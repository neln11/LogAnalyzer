#include "LogAnalyzer.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// v0.1 打印日志信息
void printLogFile(const string& filename) {
    ifstream file(filename); // 打开文件准备读取

    if (!file.is_open()) {
        cout << "无法打开日志文件: " << filename << endl;
        return;
    }

    string line;

    // 一行一行读文件，直到读完
    while (getline(file, line)) { 
        cout << line << endl;
    }

    file.close();
}

// v0.2 统计日志信息
LogStats analyzeLogStats(const std::string& filename) {
    ifstream file(filename);

    LogStats stats;
    stats.totalLines = 0;
    stats.stepLines = 0;
    stats.evalLines = 0;

    if (!file.is_open()) {
        cout << "无法打开日志文件： " << filename << endl;
        return stats;
    }

    string line;

    while (getline(file, line)) {
        stats.totalLines++;

        if (line.find("step=") != string::npos) { // string::npos 类型是size_t(一个无符号的长整型) 
                                                  // find一般找不到后会返回极大的整数值来代表找不到，
                                                  // 而string::npos为-1被转化为该类型能表示的最大数值
            stats.stepLines++;
        }

        if (line.find("[eval]") != string::npos) {
            stats.evalLines++;
        }
    }

    file.close();

    return stats;
}

void printLogStats(const LogStats& stats) {
    cout << endl;
    cout << "====== Log Statistics ======" << endl;
    cout << "Total lines: " << stats.totalLines << endl;
    cout << "Step lines: " << stats.stepLines << endl;
    cout << "Eval lines:" << stats.evalLines << endl;
}

TrainInfo analyzeTrainInfo(const std::string& filename) {
    ifstream file(filename);

    TrainInfo info;
    info.finalLoss = 0;
    info.finalStep = 0;
    info.found = 0;

    if (!file.is_open()) {
        cout << "无法打开日志文件: " << filename << endl;
        return info;
    }

    string line;

    while (getline(file, line)) {
        size_t stepPos = line.find("step=");
        size_t lossPos = line.find("loss=");

        if (stepPos != string::npos && lossPos != string::npos) {
            string stepStr = line.substr(stepPos + 5, lossPos - (stepPos + 5)); // substr用来截断字符串
            string lossStr = line.substr(lossPos + 5);

            info.finalStep = stoi(stepStr);
            info.finalLoss = stod(lossStr);
            info.found = true;
        }
    }

    file.close();

    return info;
}

void printTrainInfo(const TrainInfo& info) {
    cout << endl;
    cout << "====== Train Info ======" << endl;

    if (!info.found) {
        cout << "没有找到 step/loss 训练信息" << endl;
        return;
    }

    cout << "Final step: " << info.finalStep << endl;
    cout << "Final loss: " << info.finalLoss << endl;
}