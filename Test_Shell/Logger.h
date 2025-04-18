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

    std::string getTimestampedFileName();
    bool isFileSizeOverTenKb(const std::string& filePath);
    void renameFile(const std::string& oldName, const std::string& newName);

    
private:
    std::vector<LogEntry> logs;
    size_t currentSize = 0;
    bool currentMode = 1; // TODO
    const std::string LOGFILE = "latest.txt";
    std::string oldLogFilename = "";
    void compressOldLogFile(std::string lastLogFile);
    // 마지막 로그 파일명 // until_260307_17h_12m_11s.log 
    const std::string logDir = "log"; // 로그 디렉토리 경로
    bool isExistOldLogFile();
    void setOldLogFilename(std::string filename);
    std::string getOldLogFilename(void);
};