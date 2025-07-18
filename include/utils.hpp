#pragma once

#include <filesystem>

namespace utils
{
    namespace fs = std::filesystem;
    
    fs::path vermitDir(const fs::path& repo);
    fs::path logPath(const fs::path& repo);

    fs::path findRepo();
    fs::path getRepo();
}