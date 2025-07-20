#include "commands.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
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
    fs::path init(const str& name, const str& readme)
    {
        // Create Repo Directory with name
        fs::path basePath = fs::current_path();

        fs::path dir = basePath / name;

        fs::create_directories(dir);

        //Create README.md
        if (readme == "-r")
        {
            fs::path readmePath = dir / "README.md";

            if (!fs::exists(readmePath))
            {
                std::ofstream readmeFile(readmePath);
                if (readmeFile)
                {
                    readmeFile << "# " << name << "\n\n";
                    readmeFile << "Initialized with Vermit - VCS";
                    readmeFile.close();
                }
            }
        }

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

    // Commiting
    void commit(const str& summary)
    {
        json logData = utils::loadLog(currentWorkingDir);

        if (!logData.contains("log") || !logData["log"].is_array())
        {
            logData["log"] = json::array();
        }

        if (!logData.contains("tracking") || !logData["tracking"].is_array())
        {
            logData["tracking"] = json::array();
        }

        str newID = utils::generateCommitID(logData["log"]);

        auto now = std::time(nullptr);
        std::tm* now_tm = std::localtime(&now);
        std::stringstream timeStream;
        timeStream << std::put_time(now_tm, "%b %d %Y %H:%M:%S");

        fs::path commitDir = currentWorkingDir / ".vermit" / "commits" / newID;

        fs::create_directories(commitDir);

        for (const auto& entry : logData["tracking"]) 
        {
            str trackedFilePath = entry.get<std::string>();

            fs::path src = currentWorkingDir / trackedFilePath;
            fs::path dst = commitDir / trackedFilePath;

            if (fs::exists(src)) 
            {
                fs::create_directories(dst.parent_path());

                try 
                {
                    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
                } 
                catch (const fs::filesystem_error& e) 
                {
                    std::cerr << "Failed to copy " << src << " to " << dst << ": " << e.what() << "\n";
                }

            } 
            else 
            {
                std::cerr << "Tracked file missing: " << src << "\n";
            }
        }

        json newEntry = {
            {"id", newID},
            {"summary", summary},
            {"datetime", timeStream.str()},
            {"files", logData["tracking"]}
        };

        logData["log"].push_back(newEntry);

        utils::saveLog(currentWorkingDir, logData);

        std::cout << "commited as: " << newID << " - " << summary << "\n";
    }

    void revert(const str& id)
    {
        json logData = utils::loadLog(currentWorkingDir);

        auto& commits = logData["log"];
        auto it = std::find_if(commits.begin(), commits.end(), [&](const json& entry) {return entry["id"] == id;});

        if (it == commits.end())
        {
            std::cout << RED + "commit ID not found: " + STOP << RED + id + STOP << "\n";
            return; 
        }

        fs::path commitPath = currentWorkingDir / ".vermit" / "commits" / id;

        const auto& tracked = logData["tracking"];

        for (const auto& file : tracked)
        {
            fs::path relative = file.get<str>();
            fs::path src = commitPath / relative;
            fs::path dst = currentWorkingDir / relative;

            if (fs::exists(src))
            {
                fs::create_directories(dst.parent_path());
                fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
                
                // std::cout << "reverted: " << file << "\n";
            }
            else
            {
                std::cout << RED + "missing in commit: " + STOP << file << "\n";
            }
        }

        std::cout << "reverted to commit: " << id << "\n";
    }

    void log(const str& var)
    {
        json logData = utils::loadLog(currentWorkingDir);

        if (var == "")
        {
            for (const auto& entry : logData["log"])
            {
                std::cout << "[" << entry["id"] << "] : " << entry["summary"] << " - " << entry["datetime"] << "\n";

                for (const auto& file : entry["files"])
                {
                    std::cout << "   + " << file << "\n";
                }

                std::cout << "\n";
            }
        }
        else if (var == "id")
        {
            for (const auto& entry : logData["log"])
            {
                std::cout << "- " << entry["id"] << "\n";
            }
        }
        else if (var == "summary")
        {
            for (const auto& entry : logData["log"])
            {
                std::cout << "- " << entry["summary"] << "\n";
            }
        }
        else if (var == "datetime")
        {
            for (const auto& entry : logData["log"])
            {
                std::cout << "- " << entry["datetime"] << "\n";
            }
        }
    }
}