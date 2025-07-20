#include "utils.hpp"
#include "globals.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

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

    // Generate Commit ID
    std::string generateHexcode()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 255);

        std::stringstream ss;
        for (int i = 0; i < 3; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << dist(gen);
        }

        return ss.str();
    }
    
    int countHexConflicts(const nlohmann::json& log, const std::string& hex)
    {
        int count = 0;
        for (const auto& entry : log)
        {
            if (entry.contains("id"))
            {
                std::string id = entry["id"];
                
                if (id.find("cmt-" + hex + "-") == 0)
                {
                    ++count;
                }
            }
        }

        return count;
    }

    std::string generateCommitID(const nlohmann::json& log)
    {
        std::string hex;
        int index;
        int attempts = 0;

        do
        {
            hex = generateHexcode();
            index = countHexConflicts(log, hex) + 1;
            ++attempts;

            if (attempts > 1000)
            {
                throw std::runtime_error("failed to generate unique commit ID.");
            }
        }
        while (index > 99);

        std::stringstream id;
        id << "cmt-" << hex << "-" << std::setw(2) << std::setfill('0') << index;

        return id.str();
    }
}