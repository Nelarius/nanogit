#pragma once

#include "nlrsWidget.h"

namespace nlrs
{

class ScrollPanel : public Widget
{
public:
    ScrollPanel(Widget* parent, IAllocator& allocator)
        : Widget(parent, allocator),
        scrollBarWidth_(12),
        feather_(4.f),
        radius_(3.f),
        scrollPosition_(0.2f)
    {}

    virtual ~ScrollPanel() = default;

    void onRender() override;

    inline void setScrollBarWidth(i32 width)
    {
        scrollBarWidth_ = width;
    }

    // The feather controls the size of the drop shadow of the scroll bar
    inline void setFeather(float feather)
    {
        feather_ = feather;
    }

    // The radius of the scroll bar rectangle
    inline void setRadius(float radius)
    {
        radius_ = radius;
    }

private:
    i32 scrollBarWidth_;
    float feather_;
    float radius_;
    float scrollPosition_; // varies between 0, 1
};

}
