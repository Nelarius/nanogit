#pragma once

#include "nlrsWidget.h"
#include "nlrsFontManager.h"

#include <string>

namespace nlrs
{

class TextBox : public Widget
{
public:
    TextBox(Widget* parent, IAllocator& allocator)
        : Widget(parent, allocator),
        text_(),
        font_(FontManager::InvalidHandle),
        fontSize_(24.f),
        contentBounds_()
    {}

    virtual ~TextBox() = default;

    void onRender() override;

    void setText(const char* text);
    void setFont(FontManager::Handle font);
    void setFontSize(float size);

private:
    std::string text_;
    FontManager::Handle font_;
    float fontSize_;

    Bounds2f contentBounds_;
};

}
