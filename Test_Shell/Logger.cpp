#include "Logger.h"
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::initLogFile() {
    std::ofstream file(LOGFILE, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to initialize log.txt.\n";
        return;
    }
}

bool Logger::addLog(const std::string& funcName, const std::string& msg) {
    size_t estimatedSize = sizeof(LogEntry) + msg.size();
    if (currentSize + estimatedSize > MAX_LOG_SIZE + 512) {
        return false;
    }

    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &t);

    LogEntry entry;
    entry.timestamp = tm;

    std::snprintf(entry.functionName, sizeof(entry.functionName), "%s( )", funcName.c_str());

    entry.message = msg.substr(0, 512);

    logs.push_back(entry);
    currentSize += estimatedSize;

    if (currentMode == 1) { // TODO
        printSingleLog(entry);
    }

    writeLogToFile(entry);

    return true;
}

void Logger::printSingleLog(const LogEntry& log) {
    std::cout << formatLogLine(log) << std::endl;
}

void Logger::writeLogToFile(const LogEntry& log) {
    std::ofstream file(LOGFILE, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open log.txt for writing.\n";
        return;
    }

    file << formatLogLine(log) << "\n";
}

std::string Logger::formatLogLine(const LogEntry& log) {
    std::ostringstream oss;

    oss << "[" << std::setfill('0')
        << std::right << std::setw(2) << (log.timestamp.tm_year % 100) << "."
        << std::setw(2) << (log.timestamp.tm_mon + 1) << "."
        << std::setw(2) << log.timestamp.tm_mday << " "
        << std::setw(2) << log.timestamp.tm_hour << ":"
        << std::setw(2) << log.timestamp.tm_min << "] ";

    oss << std::setfill(' ') << std::left << std::setw(30) << log.functionName;

    std::string msg = log.message;
    std::replace(msg.begin(), msg.end(), '\n', ' ');
    oss << ": " << msg;

    return oss.str();
}