#include "nlrsRepository.h"
#include "nlrsGitUtil.h"
#include "nlrsAliases.h"

#include <cstdio>
#include <cstring>
#include <utility>

namespace nlrs
{

struct LogState
{
    git_repository* repo;
    git_revwalk* walker;
    int hide;
    int sorting;
    int revisions;
};

static void pushRev(LogState *s, git_object *obj, int hide)
{
    hide = s->hide ^ hide;

    if (!s->walker)
    {
        checkError(git_revwalk_new(&s->walker, s->repo),
            "Could not create revision walker");
        git_revwalk_sorting(s->walker, s->sorting);
    }

    if (!obj)
        checkError(git_revwalk_push_head(s->walker),
            "Could not find repository HEAD");
    else if (hide)
        checkError(git_revwalk_hide(s->walker, git_object_id(obj)),
            "Reference does not refer to a commit");  // TODO: what does this do?
    else
        checkError(git_revwalk_push(s->walker, git_object_id(obj)),
            "Reference does not refer to a commit");  // TODO: what does this do?

    git_object_free(obj);
}

static int addRevision(LogState *s, const char *revstr)
{
    git_revspec revs;
    int hide = 0;

    if (!revstr)
    {
        pushRev(s, NULL, hide);
        return 0;
    }

    if (*revstr == '^')
    {
        revs.flags = GIT_REVPARSE_SINGLE;
        hide = !hide;

        if (git_revparse_single(&revs.from, s->repo, revstr + 1) < 0)
        {
            return -1;
        }
    }
    else if (git_revparse(&revs, s->repo, revstr) < 0)
    {
        return -1;
    }

    if ((revs.flags & GIT_REVPARSE_SINGLE) != 0)
    {
        pushRev(s, revs.from, hide);
    }
    else
    {
        pushRev(s, revs.to, hide);

        if ((revs.flags & GIT_REVPARSE_MERGE_BASE) != 0)
        {
            git_oid base;
            checkError(git_merge_base(&base, s->repo,
                git_object_id(revs.from), git_object_id(revs.to)),
                "Could not find merge base", revstr);
            checkError(
                git_object_lookup(&revs.to, s->repo, &base, GIT_OBJ_COMMIT),
                "Could not find merge base commit", NULL);

            pushRev(s, revs.to, hide);
        }

        pushRev(s, revs.from, !hide);
    }

    return 0;
}

static int diffLineCallback(const git_diff_delta* delta, const git_diff_hunk* hunk, const git_diff_line* line, void* payload)
{
    std::string& diffStr = *static_cast<std::string*>(payload);

    // TODO: this could use a stack allocator or something
    char* chs = new char[line->content_len + 1u];
    std::memcpy(chs, line->content, line->content_len);
    chs[line->content_len] = '\0';

    diffStr += chs;
    diffStr += '\n';

    delete[] chs;

    return 0;
}

static std::string diffToString(git_diff* diff)
{
    std::string payload;
    checkError(git_diff_foreach(diff, nullptr, nullptr, nullptr, diffLineCallback, static_cast<void*>(&payload)), "error in diffToString");

    return payload;
}

Repository::Repository(const char* pathToRepository)
    : repository_(nullptr),
    log_()
{
    checkError(git_repository_open(&repository_, pathToRepository), "Failed to open repository");
    constructLog_();
}

Repository::Repository(Repository&& other)
    : repository_(other.repository_),
    log_(std::move(other.log_))
{
    other.repository_ = nullptr;
}

Repository::~Repository()
{
    close();
}

void Repository::close()
{
    if (repository_)
    {
        git_repository_free(repository_);
        repository_ = nullptr;
    }
}

std::string Repository::diffStats() const
{
    git_diff* diff = nullptr;
    int error = git_diff_index_to_workdir(&diff, repository_, 0, 0);

    git_diff_stats* stats;
    git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);
    git_diff_stats_format_t format = GIT_DIFF_STATS_FULL;

    checkError(git_diff_get_stats(&stats, diff), "Generating stats for diff");
    checkError(git_diff_stats_to_buf(&buf, stats, format, 80), "Formatting stats");

    std::string diffStr(buf.ptr);

    git_buf_free(&buf);
    git_diff_stats_free(stats);

    return diffStr;
}

std::string Repository::diff() const
{
    git_diff* diff = nullptr;

    int error = git_diff_index_to_workdir(&diff, repository_, 0, 0);

    // TODO: diff leaks memory, it should be freed before returning

    return diffToString(diff);
}

std::string Repository::commitDiff(const LogEntry& entry) const
{
    git_commit* commit = nullptr;
    checkError(git_commit_lookup(&commit, repository_, &entry.oid), "Failed to lookup commit: ");

    git_commit* parent = nullptr;
    checkError(git_commit_parent(&parent, commit, 0), "Failed to get commit parent: ");

    git_tree* commitTree = nullptr;
    git_tree* parentTree = nullptr;
    checkError(git_commit_tree(&commitTree, commit), "Failed to construct commit tree: ");
    checkError(git_commit_tree(&parentTree, parent), "Failed to construct commit tree: ");

    git_diff* diff = nullptr;
    checkError(git_diff_tree_to_tree(&diff, repository_, parentTree, commitTree, nullptr), "Failed to diff tree objects: ");

    // TODO: diff leaks memory, it should be freed

    return diffToString(diff);
}

std::string Repository::diffIndexToWorkDir() const
{
    git_diff* diff = nullptr;

    checkError(git_diff_index_to_workdir(&diff, repository_, nullptr, nullptr), "Failed to construct diff between index and work dir: ");

    return diffToString(diff);
}

void Repository::constructLog_()
{
    LogState logState = { 0 };
    logState.repo = repository_;

    git_commit* commit = nullptr;
    git_oid oid;

    if (!logState.revisions)
    {
        addRevision(&logState, nullptr);
    }

    for (; !git_revwalk_next(&oid, logState.walker); git_commit_free(commit))
    {
        checkError(git_commit_lookup(&commit, repository_, &oid), "Failed to look up commit");

        const git_signature* sig = nullptr;
        sig = git_commit_author(commit);

        // here's how you would figure out if there are branches in the commit graph
        //const int nparents = git_commit_parentcount(commit);
        // for each parent, do
        //git_oid parentId = *git_commit_parent_id(commit, i);
        // now you could fetch the commit representation from the graph based on its
        // representation, or something...

        log_.emplace_back(
            sig->name,
            sig->email,
            git_commit_message(commit),
            oid
        );
    }
}

}
