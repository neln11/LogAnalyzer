#include "LogAnalyzer.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
