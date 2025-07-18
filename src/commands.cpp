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

        if (fs::exists(vermitPath))
        {
            std::cout << "vermit already initialized\n";
            return dir;
        }

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
            std::cerr << "failed to initialize vermit repo\n";
        }

        return dir;
    }

    void create(const str& filename)
    {
        try
        {
            fs::path filePath = currentWorkingDir / filename;

            if (fs::exists(filePath))
            {
                std::cout << "file already exists: " << filePath << "\n";
                return;
            }

            if (!filePath.parent_path().empty()) fs::create_directories(filePath.parent_path());

            std::ofstream file(filePath);
            if (!file.is_open())
            {
                std::cout << "failed to open: " << filePath << "\n";
                return;
            }

            file << "";
            file.close();

            std::cout << "created file: " << filePath << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "error: " << e.what() << "\n";
            return;
        }

    }
}