#pragma once

#include <string>
#include <vector>
#include "json.hpp"

namespace logs
{
    using str = std::string;

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