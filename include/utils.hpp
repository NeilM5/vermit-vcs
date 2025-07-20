#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#include "json.hpp"

namespace utils
{
    namespace fs = std::filesystem;

    // Paths    
    fs::path vermitDir(const fs::path& repo);
    fs::path logPath(const fs::path& repo);

    // Load and Save
    nlohmann::json loadLog(const fs::path& repo);
    void saveLog(const fs::path& repo, const nlohmann::json& logData);

    // Commit ID Generation
    std::string generateHexcode();
    int countHexConflicts(const nlohmann::json& log, const std::string& hex);
    std::string generateCommitID(const nlohmann::json& log);
}