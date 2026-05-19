#ifndef LOG_ANALYZER_H
#define LOG_ANALYZER_H

#include <string>

// 日志统计结果结构体
struct LogStats {
    int totalLines;
    int stepLines;
    int evalLines;
};

struct TrainInfo {
    int finalStep;
    double finalLoss;
    bool found;
};

// v0.1: 读取日志文件， 并逐行打印
void printLogFile(const std::string& filename);

// v0.2: 统计日志信息
LogStats analyzeLogStats(const std::string& filename);
void printLogStats(const LogStats& stats);

// v0.3: 提取最后一个 step 和 loss
TrainInfo analyzeTrainInfo(const std::string& filename);
void printTrainInfo(const TrainInfo& info);

#endif