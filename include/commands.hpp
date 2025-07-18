#pragma once

#include <string>
#include <filesystem>

namespace cmds
{
    std::filesystem::path init(const std::string& name);
    void create(const std::string& filename);
    void mkdir(const std::string& foldername);
}