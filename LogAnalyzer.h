#ifndef LOG_ANALYZER_H
#define LOG_ANALYZER_H

#include <string>
#include <vector>

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

struct EvalInfo {
    std::string checkpoint;
    double cleanSR;
    double asr;
    double targetSR;
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

// v0.4: 使用正则表达式regex提取eval指标
EvalInfo analyzeEvalInfo(const std::string& filename);
void printEvalInfo(const EvalInfo& info);

// v0.5：分析所有eval日志
std::vector<EvalInfo> analyzeAllEvalInfos(const std::string& filename);
EvalInfo findBestASR(const std::vector<EvalInfo>& evalInfos);
void printBestASR(const EvalInfo& info);

// v0.6: 导出结果至CSV文件
void saveSummaryToCSV(const std::string& outputFilename,
                    const std::string& logFilename,
                    const LogStats& stats,
                    const TrainInfo& trainInfo,
                    const EvalInfo& bestInfo);

#endif