#include "nlrsMath.h"
#include "nlrsTextBox.h"
#include "fs/nlrsFileSystem.h"
#include "nanovg.h"

namespace nlrs
{

void TextBox::onRender()
{
    nvgFontFaceId(context_, font_);
    nvgFontSize(context_, fontSize_);
    // TODO:a background fill color should be a property
    // TODO: fill color should be a property
    nvgFillColor(context_, nvgRGB(224, 224, 224));

    auto bounds = size();
    auto extent = bounds.extent();

    float lineHeight = fontSize_;

    nvgTextAlign(context_, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgTextMetrics(context_, nullptr, nullptr, &lineHeight);

    NVGtextRow rows[3];
    int nrows = 0;
    Vec2f linePos{ float(bounds.min.x), float(bounds.min.y) };
    const char* start = text_.c_str();
    const char* end = start + text_.length();

    while (nrows = nvgTextBreakLines(context_, start, end, extent.x, rows, 3))
    {
        for (int i = 0; i < nrows; ++i)
        {
            NVGtextRow& row = rows[i];
            nvgText(context_, linePos.x, linePos.y, row.start, row.end);
            linePos.y += lineHeight;
        }
        start = rows[nrows - 1].next;
    }
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
