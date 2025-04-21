#pragma once
#include "CommandBase.h"

class HelpCommand : public CommandBase {
public:
    HelpCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>&) override {
		using std::left;
		using std::setw;
		using std::setfill;

		//ADD_LOG("ITestShell::help", "print HELP message");

		cout << "ÆÀ¸í: A class\n";
		cout << "ÆÀ¿ø: ÃÖÀç¹Î, ÃÖÀ¯Á¤, ¼Òº´¿í, ±èÈñÁ¤, ±èÃæÈñ\n";

		cout << "\n=============================== command ===============================\n";
		cout << left << setfill(' ') << setw(35) << "write [LBA] [Value]" << "Write(data) at LBA(lba_num).ex: write 3 0xAAAABBBB\n";
		cout << left << setfill(' ') << setw(35) << "read [LBA]" << "Read data at LBA(lba_num).ex: read 3\n";
		cout << left << setfill(' ') << setw(35) << "exit" << "Exit from this program.ex: exit\n";
		cout << left << setfill(' ') << setw(35) << "help" << "Display help information.ex: help\n";
		cout << left << setfill(' ') << setw(35) << "fullwrite [Value]" << "Fill all LBA with(data).ex: fullwrite 0xAAAABBBB\n";
		cout << left << setfill(' ') << setw(35) << "fullread" << "Read all LBA data and display.ex: fullread\n";

		cout << "\n=============================== TC command ===============================\n";
		cout << left << setfill(' ') << setw(35) << "1_FullWriteAndReadCompare or 1_" << "Write all LBA and test all data is written with right data.\n";
		cout << left << setfill(' ') << setw(35) << "2_PartialLBAWrite or 2_" << "Write 5 LBAs and test all data is written with right data.Repeat 30 times.\n";
		cout << left << setfill(' ') << setw(35) << "3_WriteReadAging of 3_" << "Write LBA 0 and 99 and test all data is written with right data.Repeat 200 times.\n";
		cout << left << setfill(' ') << setw(35) << "4_EraseAndWriteAging or 4_" << "For each 3 LBAs, write and erase and test. Repeat 30 times.\n";
        return COMMAND_SUCCESS;
    }
};