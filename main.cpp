#include <iostream>
#include <string>

#include "LogAnalyzer.h"

using namespace std;

int main() {
    string filename = "sample_logs/train.log";

    printLogFile(filename);

    return 0;
}