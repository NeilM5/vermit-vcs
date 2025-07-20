#include "utils.hpp"
#include "globals.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include "json.hpp"

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

    nlohmann::json loadLog(const fs::path& repo)
    {
        fs::path path = logPath(repo);
        nlohmann::json logData;

        if (fs::exists(path))
        {
            std::ifstream inFile(path);
            inFile >> logData;
        }

        else
        {
            logData["log"] = nlohmann::json::array();
            logData["tracking"] = nlohmann::json::array();
        }

        return logData;
    }

    void saveLog(const fs::path& repo, const nlohmann::json& logData)
    {
        fs::path path = logPath(repo);
        fs::create_directories(path.parent_path());

        std::ofstream outFile(path);
        outFile << logData.dump(4);
    }
}