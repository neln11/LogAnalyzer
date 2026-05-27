#include "LogAnalyzer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

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

EvalInfo analyzeEvalInfo(const std::string& filename) {
    ifstream file(filename);

    EvalInfo info;
    info.cleanSR = 0.0;
    info.asr = 0.0;
    info.targetSR = 0.0;
    info.found = false;

    if (!file.is_open()) {
        cout << "无法打开日志文件: " << filename << endl;
        return info;
    }

    string line;

    /*
    R"(...)" 是原始字符串 Raw String Literal 允许你少写很多转义符
    否则就会变成 "\\[eval\\] checkpoint=(\\S+)"
    \S+： \S 非空白字符 + 一个或多个 
    (\S+) 即匹配一串非空格字符
    \d 数字 . 小数点 [] 字符集合，即这个池子里装的东西只要匹配到任何一个就可以
    [\d.] 数字或小数点，[]里面装了一个\d 一个.
    */
    regex pattern(
        R"(\[eval\] checkpoint=(\S+) clean_sr=([\d.]+) asr=([\d.]+) target_sr=([\d.]+))"
    ); 

    /*
    regex中 (...)叫捕获组，capture group，每一组括号都会被保存，因为之前的R"(...)"里有 (...)
    所以 matches[1]就是step_500
    */
    smatch matches;

    while (getline(file, line)) {
        if (regex_search(line, matches, pattern)) {
            info.checkpoint = matches[1];

            info.cleanSR = stod(matches[2]);
            info.asr = stod(matches[3]);
            info.targetSR = stod(matches[4]);

            info.found = true;
        }
    }

    file.close();

    return info;
}

void printEvalInfo(const EvalInfo& info) {
    cout << endl;
    cout << "====== Eval Info ======" << endl;

    if (!info.found) {
        cout << "没有找到 eval 信息" << endl;
        return;
    }

    cout << "Checkpoint: " << info.checkpoint << endl;
    cout << "Clean SR: " << info.cleanSR << endl;
    cout << "ASR: " << info.asr << endl;
    cout << "Target SR: " << info.targetSR << endl;
}

vector<EvalInfo> analyzeAllEvalInfos(const string& filename) {
    ifstream file(filename);

    vector<EvalInfo> evalInfos;

    if (!file.is_open()) {
        cout << "无法打开日志文件： " << filename << endl;
        return evalInfos;
    }

    string line;

    regex pattern(
        R"(\[eval\] checkpoint=(\S+) clean_sr=([\d.]+) asr=([\d.]+) target_sr=([\d.]+))"
    );

    smatch matches;

    while (getline(file, line)) {
        if (regex_search(line, matches, pattern)) {
            EvalInfo info;

            info.checkpoint = matches[1].str();
            
            info.cleanSR = stod(matches[2]);
            info.asr = stod(matches[3]);
            info.targetSR = stod(matches[4]);

            info.found = true;

            evalInfos.push_back(info);
        }
    }

    file.close();

    return evalInfos;
}

EvalInfo findBestASR(const vector<EvalInfo>& evalInfos) {
    EvalInfo bestInfo;
    bestInfo.checkpoint = "";
    bestInfo.cleanSR = 0.0;
    bestInfo.asr = 0.0;
    bestInfo.targetSR = 0.0;
    bestInfo.found = false;

    if (evalInfos.empty()) {
        return bestInfo;
    }

    bestInfo = evalInfos[0];

    for (int i = 1; i < evalInfos.size(); i++) {
        if (evalInfos[i].asr > bestInfo.asr) {
            bestInfo = evalInfos[i];
        }
    }

    return bestInfo;
}

void printBestASR(const EvalInfo& info) {
    cout << endl;
    cout << "====== Best ASR eval======" << endl;

    if (!info.found) {
        cout << "没有找到eval信息，无法统计 Best ASR." << endl;
        return;
    }

    cout << "checkpoint: " << info.checkpoint << endl;
    cout << "cleanSR: " << info.cleanSR << endl;
    cout << "ASR: " << info.asr << endl;
    cout << "TargetSR: " << info.targetSR << endl;
}

void saveSummaryToCSV(const std::string& outputFilename,
                    const std::string& logFilename,
                    const LogStats& stats,
                    const TrainInfo& trainInfo,
                    const EvalInfo& bestInfo) {
    ofstream file(outputFilename);

    if (!file.is_open()) {
        cout << "无法打开日志文件：" << logFilename << endl;
    }


    file << "file,total_lines,step_lines,eval_lines,"
         << "final_step,final_loss,"
         << "best_checkpoint,best_clean_sr,best_asr,best_target_sr\n";

    file << logFilename << ","
         << stats.totalLines << ","
         << stats.stepLines << ","
         << stats.evalLines << ",";

    if (trainInfo.found) {
        file << trainInfo.finalStep << ","
             << trainInfo.finalLoss << ",";
    } else {
        file << "NA,NA,";
    }

    if (bestInfo.found) {
        file << bestInfo.checkpoint << ","
             << bestInfo.cleanSR << ","
             << bestInfo.asr << ","
             << bestInfo.targetSR << "\n";
    } else {
        file << "NA,NA,NA,NA\n";
    }

    file.close();

    cout << endl;
    cout << "summary 已保存到: " << outputFilename << endl;
    }
