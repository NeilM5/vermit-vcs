#include "commands.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "json.hpp"

#include "globals.hpp"

#include "logs.hpp"
#include "utils.hpp"
#include "error.hpp"

namespace fs = std::filesystem;

using str = std::string;
using json = nlohmann::json;

namespace cmds
{
    fs::path init(const str& name)
    {
        // Create Repo Directory with name
        fs::path basePath = fs::current_path();

        fs::path dir = basePath / name;

        fs::create_directories(dir);

        //Create .vermit with commits inside
        fs::path vermitPath = utils::vermitDir(dir);

        if (errors::pathExists(vermitPath, "vermit already exists")) return dir;

        fs::create_directories(vermitPath / "commits");

        //Create and serialize log.json
        logs::Log initialLog;
        std::ofstream logFile(utils::logPath(dir));

        if (logFile)
        {
            logFile << json(initialLog).dump(4);
            logFile.close();

            std::cout << "initialized vermit repo: " << dir << "\n";
        }
        else
        {
            std::cout << RED + "failed to initialize vermit repo\n" + STOP;
        }

        return dir;
    }

    void create(const str& filename)
    {
        try
        {
            fs::path filePath = currentWorkingDir / filename;

            if (errors::pathExists(filePath, "file already exists")) return;

            if (!filePath.parent_path().empty()) fs::create_directories(filePath.parent_path());

            std::ofstream file(filePath);
            if (!file.is_open())
            {
                std::cout << RED + "failed to open: " + STOP << RED + filePath.string() + STOP<< "\n";
                return;
            }

            file << "";
            file.close();

            std::cout << "created file: " << filePath.string() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << RED + "error: " + STOP << RED + e.what() + STOP<< "\n";
            return;
        }

    }

    void mkdir(const str& foldername)
    {
        try
        {
            fs::path folderPath = currentWorkingDir / foldername;

            if (errors::pathExists(folderPath, "directory already exists")) return;

            fs::create_directories(folderPath);

            std::cout << "created directory: " << folderPath.string() << "\n";
        }
        catch(const std::exception& e)
        {
            std::cerr << RED + "error: " + STOP << RED + e.what() + STOP<< "\n";
            return;
        }
        
    }

    void track(const str& file)
    {
        json logData = utils::loadLog(currentWorkingDir);
        auto& tracking = logData["tracking"];

        if (file == "all")
        {
            int added = 0;

            for (const auto& entry : fs::recursive_directory_iterator(currentWorkingDir))
            {
                if (entry.is_regular_file())
                {
                    fs::path relPath = fs::relative(entry.path(), currentWorkingDir);

                    if (relPath.string().rfind(".vermit", 0) == 0) continue;

                    if (std::find(tracking.begin(), tracking.end(), relPath.string()) != tracking.end()) continue;

                    tracking.push_back(relPath.string());
                    std::cout << "tracking: " << relPath.string() << "\n";
                    added++;
                }
            }

            if (added == 0) std::cout << "no new files to track\n";

            utils::saveLog(currentWorkingDir, logData);
            return;
        }

        fs::path path = currentWorkingDir / file;

        if (!fs::exists(path))
        {
            std::cout << "file does not exist: " << path.string() << "\n";
            return;
        }

        str relPath = fs::relative(path, currentWorkingDir).string();

        if (std::find(tracking.begin(), tracking.end(), relPath) != tracking.end())
        {
            std::cout << "already tracking file: " << relPath << "\n";
            return;
        }

        tracking.push_back(relPath);
        std::cout << "tracking: " << relPath << "\n";

        utils::saveLog(currentWorkingDir, logData);
    }
}