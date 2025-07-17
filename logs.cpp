#include <iostream>
#include <string>
#include <vector>
#include "include/json.hpp"

using str = std::string;
using json = nlohmann::json;

namespace logs
{
    struct CommitEntry
    {
        int index;
        str summary;
        
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CommitEntry, index, summary)
    };

    struct Log
    {
        std::vector<CommitEntry> log;
        std::vector<str> tracking;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Log, log, tracking)
    };
}