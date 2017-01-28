#pragma once

#include "fs/nlrsPath.h"

#include <unordered_map>

struct NVGcontext;

namespace nlrs
{

// TODO: this should use interned strings instead of strings in the hash map
class FontManager
{
public:
    using Handle = int;
    static constexpr int InvalidHandle{ 0x7FFFFFFF };

    explicit FontManager(NVGcontext* context);
    ~FontManager() = default;
    FontManager(FontManager&&);

    FontManager() = delete;
    FontManager(const FontManager&) = delete;
    FontManager& operator=(FontManager&&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    void setContext(NVGcontext*);

    Handle load(const char* fontName, const std::string& path);

private:
    NVGcontext* context_;
    std::unordered_map<std::string, Handle> fonts_;
};

}
