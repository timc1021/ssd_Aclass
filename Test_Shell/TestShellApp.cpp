#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <string>
#include <vector>

#include "TestShellApp.h"
#include "ITestScript.h"
#include "Logger.h"

void TestShellApp::run(std::istream& in, std::ostream& out)
{
    bool result = init();
    if (result == false)
        return;

    while (true) {
        std::string command;

        out << "shell> ";

        if (!std::getline(in, command)) break;
        if (command.empty()) continue;

        COMMAND_RESULT result = testShell->handleCommand(command);
        if (result == COMMAND_INVALID_PARAM)
        {
            out << "INVALID COMMAND\n";
        }
        else if (result == COMMAND_EXIT)
        {
            break;
        }

        out << "\n";
    }
}

bool TestShellApp::init()
{
    Logger::getInstance().initLogFile();

    using CreateFn = ITestScript * (*)();

    if (!isValidDllFolderPath()) return false;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(scriptPath)) {
            if (entry.path().extension() == ".dll") {
                HMODULE dll = LoadLibraryA(entry.path().string().c_str());
                if (!dll) {
                    std::cerr << "Failed to load DLL: " << entry.path() << std::endl;
                    return false;;
                }

                CreateFn create = (CreateFn)GetProcAddress(dll, "create");
                if (!create) {
                    std::cerr << "create() not found in: " << entry.path() << std::endl;
                    FreeLibrary(dll);
                    return false;;
                }

                ITestScript* script = create();
                if (!script) {
                    std::cerr << "create() 호출은 성공했지만 nullptr을 반환함: " << entry.path() << std::endl;
                    FreeLibrary(dll);
                    return false;;
                }

                script->setShell(testShell);
                for (const std::string& cmd : script->names()) {
                    testShell->registerCommand(cmd, script);
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "파일 시스템 예외 발생!\n";
        std::cerr << "→ " << e.what() << "\n";
        std::cerr << "→ 경로: " << e.path1() << "\n";
        return false;
    }

    return true;
}

bool TestShellApp::isValidDllFolderPath()
{
    if (!std::filesystem::exists(scriptPath)) {
        std::cerr << "scripts 폴더가 존재하지 않습니다: " << scriptPath << std::endl;
        return false;
    }
    return true;
}

void TestShellApp::runner(char* argv)
{
    bool result = false;
    std::string command;
    std::string file_name = argv;

    result = init();
    if (result == false)
        return;

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << file_name << std::endl;
        return;
    }

    while(!file.eof()) {
        getline(file, command);
        std::cout << command << "---   RUN   ...";

        std::streambuf* originalCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);

        result = testShell->handleCommand(command);

        std::cout.rdbuf(originalCoutBuffer);

        if (result == COMMAND_SUCCESS) {
            std::cout << "Pass\n";
        }
        else {
            std::cout << "Fail!\n";
            file.close();
            return;
        }
    }

    file.close();
}
