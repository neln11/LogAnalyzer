#include <iostream>
#include <string>
#include <vector>

#include "LogAnalyzer.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename;
    string outputFilename = "output/log.csv";

    if (argc < 2) {
        cout << "用法： " << argv[0] << " <log_file> [output_csv]" << endl;
        cout << "示例： " << argv[0] << " sample_logs/train.log" << endl;
        cout << "示例： " << argv[0] << " sample_logs/train.log output/summary.csv" << endl;
    }

    filename = argv[1];
    
    if (argc >= 3) {
        outputFilename = argv[2];
    }

    cout << "输入日志文件： " << filename << endl;
    cout << "输出 CSV 文件： " << outputFilename << endl;

    cout << endl;

    cout << "====== Log Content ======" << endl;
    printLogFile(filename);

    LogStats stats;
    stats = analyzeLogStats(filename);
    printLogStats(stats);

    TrainInfo trainInfo = analyzeTrainInfo(filename);
    printTrainInfo(trainInfo);

    EvalInfo evalInfo = analyzeEvalInfo(filename);
    printEvalInfo(evalInfo);

    vector<EvalInfo> evalInfos = analyzeAllEvalInfos(filename);
    EvalInfo bestInfo = findBestASR(evalInfos);
    printBestASR(bestInfo);

    saveSummaryToCSV(outputFilename, filename, stats, trainInfo, bestInfo);
    
    return 0;
}