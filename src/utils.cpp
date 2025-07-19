#include "utils.hpp"
#include "globals.hpp"

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
}