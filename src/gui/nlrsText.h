#pragma once

#include "nlrsFontManager.h"

#include <string>

namespace nlrs
{

struct Text
{
    Text()
        : text(""),
        fontId(FontManager::InvalidHandle),
        fontSize(18.f)
    {}

    std::string text;
    FontManager::Handle fontId;
    float fontSize;
};

}
