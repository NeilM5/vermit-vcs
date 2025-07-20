#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <iomanip>

#include "globals.hpp"

#include "commands.hpp"
#include "utils.hpp"
#include "error.hpp"

using str = std::string;
namespace fs = std::filesystem;

fs::path currentWorkingDir = fs::current_path();

std::vector<str> tokenize(const str& input)
{
    std::istringstream iss(input);
    std::vector<str> tokens;
    str token;

    while (iss >> std::quoted(token))
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
        if (!errors::checkArgsSize(args, 2, "cwd <path>")) return;
        
        fs::path newPath = fs::absolute(args[1]);

        if (fs::exists(newPath) && fs::is_directory(newPath))
        {
            currentWorkingDir = newPath;
            std::cout << "current directory changed to: " << newPath.string() << "\n";
        }
        else
        {
            errors::singleLineError("directory not found: ", newPath.string());
        }
    }

    else if (args[0] == "vermit")
    {
        if (!errors::checkArgsSize(args, 2, "vermit <command> [args...]")) return;

        str subCmd = args[1];

        if (subCmd == "init")
        {
            if (!errors::checkArgsSize(args, 3, "vermit init <name> -r")) return;

            fs::path prev = fs::current_path();
            fs::current_path(currentWorkingDir);

            fs::path repoPath = cmds::init(args[2], args.size() > 3 ? args[3] : "");

            fs::current_path(prev);

            currentWorkingDir = repoPath;
            std::cout << "current directory changed to: " << currentWorkingDir.string() << "\n";
        }
        else if (subCmd == "create")
        {
            if (!errors::checkArgsSize(args, 3, "vermit create <file_name>")) return;
            cmds::create(args[2]);
        }
        else if (subCmd == "mkdir")
        {
            if (!errors::checkArgsSize(args, 3, "vermit mkdir <folder_name>")) return;
            cmds::mkdir(args[2]);
        }
        else if (subCmd == "track")
        {
            if (!errors::checkArgsSize(args, 3, "vermit track <filename>")) return;
            cmds::track(args[2]);
        }
        else if (subCmd == "commit")
        {
            if (!errors::checkArgsSize(args, 3, "vermit commit <summary>")) return;
            cmds::commit(args[2]);
        }
        else if (subCmd == "log")
        {
            if (!errors::checkArgsSize(args, 2, "vermit log")) return;
            cmds::log(args.size() > 2 ? args[2] : "");
        }
        else
        {
            errors::singleLineError("invalid vermit command: ", subCmd);
        }
    }

    else
    {
        errors::singleLineError("invalid command: ", args[0]);
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