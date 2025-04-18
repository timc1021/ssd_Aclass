
#include "FileTextIOInterface.h"
#include "CommandBuffer.h"
#include <gmock/gmock.h>
#include <filesystem>
#include "CommandBufferTest.h"
namespace fs = std::filesystem;
class CommandBufferTest : public ::testing::Test {
public:

	void removeFilesAt(std::string dirPath) {
		for (const auto& entry : fs::directory_iterator(dirPath)) {
			if (entry.is_regular_file()) {
				fs::remove(entry.path());
			}
		}
	}

};

//TEST_F(CommandBufferTest, tc) {
//	CommandBuffer buffer;
//	std::string strCommand = "W 20 0xABCDABCD";
//	CommandValue command(strCommand);
//
//	buffer.printBuffer();
//
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//}

//TEST_F(CommandBufferTest, tc2) {
//	CommandBuffer buffer;
//	std::string strCommand = "W 20 0xABCDABCD";
//	CommandValue command(strCommand);
//	removeFilesAt("./buffer");
//
//	buffer.printBuffer();
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "W 20 0xFFFFFFFF";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//}

//TEST_F(CommandBufferTest, tc2) {
//	removeFilesAt("./buffer");
//	CommandBuffer buffer;
//	std::string strCommand = "W 20 0xABCDABCD";
//	CommandValue command(strCommand);
//
//	buffer.printBuffer();
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "E 20 3";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//}

//TEST_F(CommandBufferTest, tc3) {
//	removeFilesAt("./buffer");
//	CommandBuffer buffer;
//	std::string strCommand = "E 20 1";
//	CommandValue command(strCommand);
//
//	buffer.printBuffer();
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "E 21 2";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "E 22 2";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "E 25 2";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//
//	strCommand = "E 20 5";
//	command.setCommand(strCommand);
//	buffer.addCommandToBuffer(command);
//	buffer.printBuffer();
//}
TEST_F(CommandBufferTest, tc3) {
	removeFilesAt("./buffer");
	CommandBuffer buffer;
	std::string strCommand = "E 20 1";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();

	strCommand = "E 21 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();

	strCommand = "E 22 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();

	strCommand = "E 25 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();

	strCommand = "E 20 5";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();

	strCommand = "E 20 5";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	buffer.printBuffer();
}