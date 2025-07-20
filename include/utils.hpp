#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#include "json.hpp"

namespace utils
{
    namespace fs = std::filesystem;
    
    fs::path vermitDir(const fs::path& repo);
    fs::path logPath(const fs::path& repo);

    nlohmann::json loadLog(const fs::path& repo);
    void saveLog(const fs::path& repo, const nlohmann::json& logData);
}