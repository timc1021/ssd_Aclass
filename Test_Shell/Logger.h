#pragma once
#include <string>
#include <vector>
#include <ctime>

using std::string;
constexpr size_t MAX_LOG_SIZE = 10240; // 10KB

struct LogEntry {
    std::tm timestamp;
    char functionName[64];
    string message;
};

class Logger {
public:
    static Logger& getInstance();

    bool initLogFile();
    void addLog(const string& funcName, const string& msg);
    void rotateLogFileIfNeeded();
    LogEntry makeLogEntry(const string& funcName, const string& msg);
    void printSingleLog(const LogEntry& log);
    void writeLogToFile(const LogEntry& log);
    string formatLogLine(const LogEntry& log);
    string getTimestampedFileName();
    bool isFileSizeOverTenKb(const string& filePath);
    void renameFile(const string& oldName, const string& newName);
    void setRunnerMode(bool runnerMode);
    
private:
    std::vector<LogEntry> logs;
    bool isRunnerMode = false;
    const string LOGFILE = "latest.txt";
    string oldLogFilename = "";
    void compressOldLogFile(string lastLogFile);
    const std::string logDir = "log";
    bool isExistOldLogFile();
    void setOldLogFilename(string filename);
    string getOldLogFilename(void);
};