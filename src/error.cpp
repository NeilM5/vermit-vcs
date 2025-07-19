#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "globals.hpp"
#include "error.hpp"

namespace errors
{
    void singleLineError(const std::string& errMessage, const std::string& message)
    {
        std::cout << RED + errMessage + STOP << RED + message + STOP << "\n";
    }

    bool pathExists(const std::filesystem::path path, const std::string& message)
    {
        if (std::filesystem::exists(path))
        {
            std::cout << RED + message + STOP << "\n";
            return true;
        }

        return false;
    }

    bool checkArgsSize(const std::vector<std::string>& args, const int& limit, const std::string& message)
    {
        if (args.size() < limit)
        {
            std::cout << RED + "usage: " + STOP << RED + message + STOP << "\n";
            return false;
        }

        return true;
    }
}