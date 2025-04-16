#include <string>
#include <vector>
class TestShell {
public:
    std::vector<std::string> splitBySpace(const std::string& input) {
        std::vector<std::string> tokens;
        size_t start = 0, end;

        while ((end = input.find(' ', start)) != std::string::npos) {
            if (end != start)
                tokens.push_back(input.substr(start, end - start));
            start = end + 1;
        }

        if (start < input.size())
            tokens.push_back(input.substr(start));

        return tokens;
    }

    unsigned int init(std::string input) {
        std::vector<std::string> result = splitBySpace(input);

        if (result[0] == "read") {
            // call read()
            return true;
        }
        else if (result[0] == "write") {
            // call write()
            return true;
        }
    }
};