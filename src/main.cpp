#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>


#include "commands.hpp"
#include "utils.hpp"

using str = std::string;
namespace fs = std::filesystem;

const str YELLOW = "\033[33m";
const str STOP = "\033[0m";

fs::path currentWorkingDir = fs::current_path();

std::vector<str> tokenize(const str& input)
{
    std::istringstream iss(input);
    std::vector<str> tokens;
    str token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void command(const std::vector<str>& args)
{
    if (args.empty()) return;

    if (args[0] == "exit")
    {
        std::exit(0);
    }

    // Set current working directory with 'cwd <path>' command
    else if (args[0] == "cwd")
    {
        if (!utils::checkArgsSize(args, 2, "cwd <path>")) return;

        fs::path newPath = fs::absolute(args[1]);

        if (fs::exists(newPath) && fs::is_directory(newPath))
        {
            currentWorkingDir = newPath;
            std::cout << "current directory changed to: " << newPath << "\n";
        }
        else
        {
            std::cout << "directory not found: " << newPath << "\n";
        }
    }

    else if (args[0] == "vermit")
    {
        if (!utils::checkArgsSize(args, 2, "vermit <command> [args...]")) return;

        str subCmd = args[1];

        if (subCmd == "init")
        {
            if (!utils::checkArgsSize(args, 3, "vermit init <name>")) return;

            fs::path prev = fs::current_path();
            fs::current_path(currentWorkingDir);

            fs::path repoPath = cmds::init(args[2]);

            fs::current_path(prev);

            currentWorkingDir = repoPath;
            std::cout << "current directory changed to: " << currentWorkingDir << "\n";
        }
        else
        {
            std::cout << "invalid vermit command: " << subCmd << "\n";
        }
    }

    else
    {
        std::cout << "invalid command: " << args[0] << "\n";
    }
}

int main(int argc, char* argv[])
{

    while (true)
    {
        std::cout << "\n[" << YELLOW + currentWorkingDir.string() + STOP << "]" << "\n" << "> ";

        str line;

        if (!std::getline(std::cin, line)) break;

        std::vector<str> args = tokenize(line);
        command(args);
    }
    

    return 0;
}