#include "nlrsMath.h"
#include "nlrsTextBox.h"
#include "fs/nlrsFileSystem.h"
#include "nanovg.h"

#include <cmath>

namespace nlrs
{

void TextBox::onRender()
{
    nvgFontFaceId(context_, font_);
    nvgFontSize(context_, fontSize_);
    // TODO:a background fill color should be a property
    // TODO: fill color should be a property
    nvgFillColor(context_, nvgRGB(224, 224, 224));

    auto b = contentBounds_;
    auto extent = b.extent();

    float lineHeight = fontSize_;

    nvgTextAlign(context_, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgTextMetrics(context_, nullptr, nullptr, &lineHeight);

    NVGtextRow rows[3];
    int nrows = 0;
    Vec2f linePos{ b.min.x, b.min.y };
    const char* start = text_.c_str();
    const char* end = start + text_.length();

    while ((nrows = nvgTextBreakLines(context_, start, end, float(extent.x), rows, 3)))
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

    nvgFontFaceId(context_, font_);
    nvgFontSize(context_, fontSize_);

    Bounds2f fbounds = Bounds2f(parent_->position(), parent_->position() + parent_->size());

    Vec2f extent = fbounds.extent();

    const char* start = text_.c_str();
    const char* end = start + text_.length();

    nvgTextBoxBounds(context_, fbounds.min.x, fbounds.min.y, extent.x, start, end, &fbounds.min.x);
    size_ = fbounds.extent();

    contentBounds_ = Bounds2f(
        Vec2f{ fbounds.min.x, fbounds.min.y },
        Vec2f { fbounds.max.x, fbounds.max.y }
    );
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
