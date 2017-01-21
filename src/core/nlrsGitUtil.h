#pragma once

#include "git2.h"
#include <cstdio>
#include <cstring>

namespace nlrs
{

inline void checkError(int error, const char* message, const char* extra = NULL)
{
    const git_error* lg2err;
    const char* lg2msg = "";
    const char* lg2spacer = "";

    if (!error)
        return;

    // giterr_last returns the latest error generated in the current thread
    // or NULL if nothing was generated
    if ((lg2err = giterr_last()) != NULL && lg2err->message != NULL)
    {
        lg2msg = lg2err->message;
        lg2spacer = " - ";
    }

    if (extra)
        std::fprintf(stderr, "%s '%s' [%d]%s%s\n",
            message, extra, error, lg2spacer, lg2msg);
    else
        std::fprintf(stderr, "%s [%d]%s%s\n",
            message, error, lg2spacer, lg2msg);

    exit(1);
}

}
