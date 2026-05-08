cmake_minimum_required(VERSION 3.9)

project(LogAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(log_analyzer
    main.cpp
    LogAnalyzer.cpp
)