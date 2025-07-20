#pragma once

#include <string>
#include <vector>
#include "json.hpp"

namespace logs
{
    using str = std::string;

    struct CommitEntry
    {
        str id;
        str summary;
        str datetime;
        std::vector<str> files;
        
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CommitEntry, id, summary, datetime, files)
    };

    struct Log
    {
        std::vector<CommitEntry> log;
        std::vector<str> tracking;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Log, log, tracking)
    };
}