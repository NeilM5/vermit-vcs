#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace utils
{
    namespace fs = std::filesystem;
    
    fs::path vermitDir(const fs::path& repo);
    fs::path logPath(const fs::path& repo);

    bool checkArgsSize(const std::vector<std::string>& args, const int& limit, const std::string& message);
}