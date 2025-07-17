#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "json.hpp"

#include "logs.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

using str = std::string;
using json = nlohmann::json;

namespace cmds
{
    void init(const str& path, const str& name)
    {
        // Create Repo Directory with name
        fs::path basePath = path;
        fs::path dirName = name;

        fs::path dir = basePath / dirName;

        fs::create_directories(dir);

        //Create .vermit with commits inside
        fs::path vermitPath = utils::vermitDir(dir);

        if (fs::exists(vermitPath))
        {
            std::cout << "vermit already initialized\n";
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
    }
}