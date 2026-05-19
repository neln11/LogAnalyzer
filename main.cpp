#include <iostream>
#include <string>

#include "LogAnalyzer.h"

using namespace std;

int main() {
    string filename = "sample_logs/train.log";

    cout << "====== Log Content ======" << endl;
    printLogFile(filename);

    LogStats stats;
    stats = analyzeLogStats(filename);
    printLogStats(stats);

    TrainInfo trainInfo = analyzeTrainInfo(filename);
    printTrainInfo(trainInfo);

    return 0;
}