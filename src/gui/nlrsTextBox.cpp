#include "nlrsTextBox.h"
#include "fs/nlrsFileSystem.h"
#include "nanovg.h"

namespace nlrs
{

void TextBox::onRender()
{
    nvgFillColor(context_, nvgRGB(224, 224, 224));
    nvgFill(context_);

    nvgFontFaceId(context_, font_);
    nvgFontSize(context_, fontSize_);

    nvgFillColor(context_, nvgRGB(32, 32, 32));
    nvgText(context_,50.f, 50.f, text_.c_str(), nullptr);
}

void TextBox::setText(const char* text)
{
    text_ = text;
}

void TextBox::setFont(FontManager::Handle handle)
{
    if (handle == FontManager::InvalidHandle)
    {
        return;
    }
    font_ = handle;
}

void TextBox::setFontSize(float size)
{
    if (size <= 0.f)
    {
        return;
    }
    fontSize_ = size;
}

}
