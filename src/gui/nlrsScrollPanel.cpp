#include "nlrsScrollPanel.h"
#include "nlrsLog.h"
#include "nlrsMath.h"
#include "nanovg.h"

namespace nlrs
{

void ScrollPanel::onRender()
{
    if (children_.empty())
    {
        return;
    }
    else if (children_.size() > 1)
    {
        LOG_ERROR << "The scroll panel had more than one child!";
        return;
    }

    Bounds2f b = Bounds2f(position_, position_ + size_);

    float x = b.min.x;
    float y = b.min.y;
    float w = size_.x;
    float h = size_.y;

    // render the scroll bar

    // TODO: expose the color of the scroll bar as external parameter
    NVGpaint paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ + 1.f, y + 1.f, scrollBarWidth_ + 1.f,
        h, radius_, feather_, nvgRGB(128, 128, 128), nvgRGB(64, 64, 64));
    nvgBeginPath(context_);
    nvgRoundedRect(context_, x + w - scrollBarWidth_, y, scrollBarWidth_,
        h, radius_);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    // TODO:
    // sum the height of the children
    Widget& child = *children_[0];
    float scrollh = h;
    float childHeight = child.size().y;
    scrollh = h * (h / childHeight);

    paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ - 1.f, y + (h - scrollh) * scrollPosition_,
        scrollBarWidth_, scrollh, radius_, feather_, nvgRGB(220, 220, 220), nvgRGB(128, 128, 128));
    nvgBeginPath(context_);
    nvgRoundedRect(
        context_, x + w - scrollBarWidth_ + 1.f, y + 1.f + (h - scrollh) * scrollPosition_,
        scrollBarWidth_ - 1.f, scrollh - 1.f, radius_ - 1.f);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    nvgSave(context_);

    // render the content of the scroll panel

    nvgScissor(context_, x, y, w - scrollBarWidth_, h);

    nvgTranslate(context_, 0.f, -child.size().y * scrollPosition_);

    child.onRender();

    nvgRestore(context_);
}

void ScrollPanel::onMouseScroll(i32 delta, Vec2i coordinates)
{
    // TODO: scale scroll factor by child content height
    scrollPosition_ = clamp(scrollPosition_ + 0.02f * -delta, 0.f, 1.f);
}

}
