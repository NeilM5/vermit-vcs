#include "utils.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace utils
{
    fs::path vermitDir(const fs::path& repo)
    {
        fs::path vermit = ".vermit";
        return repo / vermit;
    }

    fs::path logPath(const fs::path& repo)
    {
        fs::path log = "log.json";
        return vermitDir(repo) / log;
    }

    bool checkArgsSize(const std::vector<std::string>& args, const int& limit, const std::string& message)
    {
        if (args.size() < limit)
        {
            std::cout << "usage: " << message << "\n";
            return false;
        }

        return true;
    }
}