#pragma once
#include "Type.h"
#include "ICommand.h"
#include "ITestShell.h"
#include "Logger.h"

#define MAX_LBA_SIZE 100
#define MAX_ERASE_LBA (10)
#define START_LBA     (0)
#define HEX_PREFIX			("0x")
#define HEX_PREFIX_LENGTH	(2)
#define TOKEN_COMMAND		(0)
#define DATA_LENGTH         (10)
#define ADD_LOG		  (Logger::getInstance().addLog)

typedef enum {
	TOKEN_WRITE_LBA = 1,
	TOKEN_WRITE_DATA,
	TOKEN_WRITE_NUM,
} WRITE_TOKEN;

typedef enum {
	TOKEN_FULLWRITE_DATA = 1,
	TOKEN_FULLWRITE_NUM,
} FULLWRITE_TOKEN;

typedef enum {
	TOKEN_FULLREAD_NUM = 1,
} FULLREAD_TOKEN;

typedef enum {
	TOKEN_ERASE_LBA = 1,
	TOKEN_ERASE_SIZE,
	TOKEN_ERASE_NUM,
} ERASE_TOKEN;

typedef enum {
	TOKEN_FLUSH_NUM = 1,
} FLUSH_TOKEN;

typedef enum {
	TOKEN_ERASE_RANGE_START_LBA = 1,
	TOKEN_ERASE_RANGE_END_LBA,
	TOKEN_ERASE_RANGNE_NUM,
} ERASE_RANGE_TOKEN;

class CommandBase : public ICommand {
protected:
    CommandBase(ITestShell* testShell) : shell(testShell) {}
	bool isLBAValid(int32_t lba);
	bool isDataValid(const string& data);
	bool tryParseInt32(const std::string& str, int32_t& outValue);
    ITestShell* shell;
};
