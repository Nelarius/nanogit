#pragma once

#include "nlrsWidget.h"

namespace nlrs
{

class ScrollPanel : public Widget
{
public:
    ScrollPanel(Widget* parent, IAllocator& allocator)
        : Widget(parent, allocator),
        scrollBarWidth_(12.f),
        feather_(4.f),
        radius_(3.f),
        scrollPosition_(0.0f)
    {}

    virtual ~ScrollPanel() = default;

    void onRender() override;
    void onMouseScroll(i32 delta, Vec2i coordinates) override;

    inline void setScrollBarWidth(float width)
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
    float scrollBarWidth_;
    float feather_;
    float radius_;
    float scrollPosition_; // varies between 0, 1
};

}
