#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace errors
{

    void singleLineError(const std::string& errMessage, const std::string& message);
    bool pathExists(const std::filesystem::path path, const std::string& message);
    bool checkArgsSize(const std::vector<std::string>& args, const int& limit, const std::string& message);
}