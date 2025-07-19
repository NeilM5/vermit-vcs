#pragma once

#include <filesystem>
#include <string>

const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";
const std::string STOP = "\033[0m";

extern std::filesystem::path currentWorkingDir;