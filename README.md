# LogAnalyzer

A simple C++ log analyzer for reading and parsing experiment logs.

## V0.1

Read a log file and print it line by line.

## Build with g++

```bash
g++ main.cpp LogAnalyzer.cpp -o log_analyzer
./log_analyzer
```

## Build with CMake

```bash
mkdir build
cd build
cmake ..
make
cd ..
./build/log_analyzer
```