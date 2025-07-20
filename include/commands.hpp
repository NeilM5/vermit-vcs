#pragma once

#include <string>
#include <filesystem>

namespace cmds
{
    // Initialize
    std::filesystem::path init(const std::string& name, const std::string& readme = "");

    // Create Files/Folders
    void create(const std::string& filename);
    void mkdir(const std::string& foldername);

    //Tracking
    void track(const std::string& file);
}