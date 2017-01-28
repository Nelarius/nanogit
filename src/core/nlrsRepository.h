#pragma once

#include "git2.h"
#include <vector>
#include <string>

namespace nlrs {

class Repository
{
public:

    struct Commit
    {
        git_oid objectId;
    };

    struct LogEntry
    {
        LogEntry(const char* auth, const char* email, const char* msg, git_oid id)
            : author(auth), email(email), shortMessage(""), message(msg), oid(id)
        {
            // parse the first line from the commit message
            auto pos = message.find_first_of("\n");
            shortMessage = message.substr(0, pos);
        }
        std::string author;
        std::string email;
        std::string shortMessage;
        std::string message;
        git_oid     oid;
    };

    explicit Repository(const char*);
    Repository(Repository&&);
    ~Repository();

    Repository() = delete;


    void close();

    const std::vector<LogEntry>& log() const { return log_; }

    std::string diffStats() const;

    std::string diff() const;

    std::string commitDiff(const LogEntry& entry) const;

    std::string diffIndexToWorkDir() const;

private:
    void constructLog_();

    git_repository*         repository_;
    std::vector<LogEntry>   log_;
    //std::vector<Commit>     log_;
};

}
