#pragma once
#include <string>
#include <vector>
#include <ctime>

constexpr size_t MAX_LOG_SIZE = 10240; // 10KB

struct LogEntry {
    std::tm timestamp;
    char functionName[64];
    std::string message;
};

class Logger {
public:
    static Logger& getInstance();

    void initLogFile();
    bool addLog(const std::string& funcName, const std::string& msg);
    void printSingleLog(const LogEntry& log);
    void writeLogToFile(const LogEntry& log);
    std::string formatLogLine(const LogEntry& log);


private:
    std::vector<LogEntry> logs;
    size_t currentSize = 0;
    bool currentMode = 1; // TODO
    const std::string LOGFILE = "latest.txt";
};