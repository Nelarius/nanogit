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
    Vec2f linePos{ float(b.min.x), float(b.min.y) };
    const char* start = text_.c_str();
    const char* end = start + text_.length();

    while (nrows = nvgTextBreakLines(context_, start, end, float(extent.x), rows, 3))
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

Bounds2i TextBox::contentBounds() const
{
    if (text_.length() == 0)
    {
        return parent_->contentBounds();
    }
    else
    {
        return contentBounds_;
    }
}

void TextBox::setText(const char* text)
{
    text_ = text;

    Bounds2f fbounds = parent_->contentBounds().cast<float>();
    fbounds.shrink(float(padding_ + margin_ + border_));
    Vec2f extent = fbounds.extent();

    const char* start = text_.c_str();
    const char* end = start + text_.length();

    nvgTextBoxBounds(context_, fbounds.min.x, fbounds.min.y, extent.x, start, end, &fbounds.min.x);

    contentBounds_ = Bounds2i(
        Vec2i{i32(std::ceil(fbounds.min.x)), i32(std::ceil(fbounds.min.y))},
        Vec2i{i32(std::ceil(fbounds.max.x)), i32(std::ceil(fbounds.max.y))}
    );

    contentBounds_ = contentBounds_.shrink(i32(padding_ + margin_ + border_));
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
