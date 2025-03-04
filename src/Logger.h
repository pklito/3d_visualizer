// C++ program to implement a basic logging system.
#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "graphics/GUIfuncs.h"

// Enum to represent log levels
enum LogLevel { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL };

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(LogLevel level, const std::string& message);
    Logger& operator<<(const std::string& message);
    void buildGUI();

    static Logger& getInstance();

private:
    struct Log {
        LogLevel level;
        std::string appendix;
        std::string message;
    };
    std::ofstream logFile; // File stream for the log file
    std::vector<Log> logs; // Vector to store log entries
    std::string levelTostring(LogLevel level);
};

std::string generateLogFilename();