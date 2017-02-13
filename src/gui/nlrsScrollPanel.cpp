#include "nlrsScrollPanel.h"
#include "nanovg.h"

namespace nlrs
{

void ScrollPanel::onRender()
{
    Bounds2i b = bounds();
    Vec2i extent = b.extent();

    float x = float(b.min.x);
    float y = float(b.min.y);
    float w = float(extent.x);
    float h = float(extent.y);

    // render the scroll bar
    NVGpaint paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ + 1.f, y + 1.f, scrollBarWidth_ + 1.f,
        h, radius_, feather_, nvgRGB(128, 128, 128), nvgRGB(64, 64, 64));
    nvgBeginPath(context_);
    nvgRoundedRect(context_, x + w - float(scrollBarWidth_), y, float(scrollBarWidth_),
        h, radius_);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    // TODO: calculate scrollh
    float scrollh = 0.f;
    paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ - 1.f, y + (h - scrollh) * scrollPosition_,
        scrollBarWidth_, scrollh, radius_, feather_, nvgRGB(220, 220, 220), nvgRGB(128, 128, 128));
    nvgBeginPath(context_);
    nvgRoundedRect(
        context_, x + w - float(scrollBarWidth_) + 1.f, y + 1.f + (h - scrollh) * scrollPosition_,
        scrollBarWidth_ - 1.f, scrollh - 1.f, radius_ - 1.f);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    nvgSave(context_);

    // render the content of the scroll panel

    nvgScissor(context_, x, y, w - scrollBarWidth_, h);

    for (auto* child : children_)
    {
        child->onRender();
    }

    nvgRestore(context_);
}

}
