#include "utils.hpp"

#include <filesystem>
#include <iostream>

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

    fs::path findRepo()
    {
        fs::path currentPath = fs::current_path();

        while (true)
        {
            if (fs::exists(currentPath / ".vermit") && fs::is_directory(currentPath / ".vermit"))
            {
                return currentPath;
            }

            if (!currentPath.has_parent_path())
            {
                throw std::runtime_error("not a vermit repository");
            }

            currentPath = currentPath.parent_path();
        }
    }

    fs::path getRepo()
    {
        try
        {
            return findRepo();
        }
        catch (const std::exception& e)
        {
            std::cerr << "error: " << e.what() << "\n";
            std::exit(1);
        }
    }
}