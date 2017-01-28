#include "nlrsAssert.h"
#include "nlrsFontManager.h"
#include "nlrsLog.h"
#include "fs/nlrsFileSystem.h"
#include "nanovg.h"

#include <utility>

namespace nlrs
{

FontManager::FontManager(NVGcontext* context)
    : context_(context),
    fonts_()
{}

FontManager::FontManager(FontManager&& other)
    : context_(other.context_),
    fonts_(std::move(other.fonts_))
{
    other.context_ = nullptr;
}

void FontManager::setContext(NVGcontext* context)
{
    if (context == nullptr)
    {
        LOG_ERROR << "NVGcontext was null";
        return;
    }
    context_ = context;
}

FontManager::Handle FontManager::load(const char* fontName, const std::string& fontPath)
{
    auto iter = fonts_.find(fontPath);

    if (iter == fonts_.end())
    {
        if (!fs::exists(fontPath))
        {
            LOG_ERROR << "Couldn't load " << fontPath;
            return InvalidHandle;
        }

        Handle handle = nvgCreateFont(context_, fontName, fontPath.c_str());
        if (handle != InvalidHandle)
        {
            fonts_.insert(std::make_pair(fontPath, handle));
        }
        return handle;
    }
    else
    {
        return iter->second;
    }
}

}
