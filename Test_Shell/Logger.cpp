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
#include <filesystem>

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

    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &t);

    LogEntry entry;
    entry.timestamp = tm;

    std::snprintf(entry.functionName, sizeof(entry.functionName), "%s( )", funcName.c_str());

    entry.message = msg.substr(0, 512);

    logs.push_back(entry);

    printSingleLog(entry);

    if (isFileSizeOverTenKb(LOGFILE) == true) {
        std::string newFilename = getTimestampedFileName();
    
        renameFile(LOGFILE, newFilename);

        if (isExistOldLogFile() == true) {
            compressOldLogFile(getOldLogFilename());
            setOldLogFilename(newFilename);
        }
        else {
            setOldLogFilename(newFilename);
        }
    }
    writeLogToFile(entry);

    return true;
}

void Logger::printSingleLog(const LogEntry& log) {
    if (isRunnerMode == true)
        return;

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

std::string Logger::getTimestampedFileName() {
    std::filesystem::create_directories(logDir);
    std::time_t now = std::time(nullptr);
    std::tm timeInfo;

    localtime_s(&timeInfo, &now);

    std::ostringstream oss;
    oss << "until_"
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_year % 100)
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_mon + 1) 
        << std::setw(2) << std::setfill('0') << timeInfo.tm_mday
        << "_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_hour << "h_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_min << "m_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_sec << "s"
        << ".log";

    return logDir + "/" + oss.str();
}

bool Logger::isFileSizeOverTenKb(const std::string& filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0) {      
        return false;
    }
    return fileStat.st_size >= MAX_LOG_SIZE;
}

void Logger::renameFile(const std::string& oldName, const std::string& newName) {
    if (std::rename(oldName.c_str(), newName.c_str()) != 0) {
        std::perror(("Failed to rename " + oldName + " to " + newName).c_str());
    }
    else {
        addLog("Logger::renameFile", "Renamed " + oldName + "��" + newName);
    }
}

void Logger::setMode(bool runnerMode)
{
    isRunnerMode = runnerMode;
}

void Logger::compressOldLogFile(std::string lastLogFile) {
    try {
        std::filesystem::path oldLogPath(lastLogFile);
        std::filesystem::path zipName = oldLogPath;
        zipName.replace_extension(".zip");

        if (!std::filesystem::exists(oldLogPath)) {
            std::cerr << "Old log file does not exist: " << oldLogPath << std::endl;
            return;
        }

        std::filesystem::rename(oldLogPath, zipName);
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to compress log file: " << e.what() << std::endl;
        std::abort();
    }
}

bool Logger::isExistOldLogFile()
{
    if (oldLogFilename == "")
        return false;
    else
        return true;
}

void Logger::setOldLogFilename(std::string filename)
{
    oldLogFilename = filename;
}

std::string Logger::getOldLogFilename(void) {
    return oldLogFilename;
}
