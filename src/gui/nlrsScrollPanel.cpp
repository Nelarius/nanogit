#include "nlrsScrollPanel.h"
#include "nlrsLog.h"
#include "nlrsMath.h"
#include "nanovg.h"

namespace nlrs
{

void ScrollPanel::onRender()
{
    Bounds2i b = contentBounds();
    Vec2i extent = b.extent();

    float x = float(b.min.x);
    float y = float(b.min.y);
    float w = float(extent.x);
    float h = float(extent.y);

    // render the scroll bar

    // TODO: expose the color of the scroll bar as external parameter
    NVGpaint paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ + 1.f, y + 1.f, scrollBarWidth_ + 1.f,
        h, radius_, feather_, nvgRGB(128, 128, 128), nvgRGB(64, 64, 64));
    nvgBeginPath(context_);
    nvgRoundedRect(context_, x + w - float(scrollBarWidth_), y, float(scrollBarWidth_),
        h, radius_);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    float scrollh = h;
    if (child_)
    {
        float childHeight = child_->contentBounds().extent().cast<float>().y;
        scrollh = h * (h / childHeight);
    }

    paint = nvgBoxGradient(
        context_, x + w - scrollBarWidth_ - 1.f, y + (h - scrollh) * scrollPosition_,
        float(scrollBarWidth_), scrollh, radius_, feather_, nvgRGB(220, 220, 220), nvgRGB(128, 128, 128));
    nvgBeginPath(context_);
    nvgRoundedRect(
        context_, x + w - float(scrollBarWidth_) + 1.f, y + 1.f + (h - scrollh) * scrollPosition_,
        scrollBarWidth_ - 1.f, scrollh - 1.f, radius_ - 1.f);
    nvgFillPaint(context_, paint);
    nvgFill(context_);

    nvgSave(context_);

    // render the content of the scroll panel

    nvgScissor(context_, x, y, w - scrollBarWidth_, h);

    if (child_)
    {
        auto b = child_->contentBounds();
        float childHeight = b.extent().cast<float>().y + 0.5f * contentBounds().extent().y;

        nvgTranslate(context_, 0.f, -childHeight * scrollPosition_);
    }

    child_->onRender();

    nvgRestore(context_);
}

void ScrollPanel::onMouseScroll(i32 delta, Vec2i coordinates)
{
    // TODO: scale scroll factor by child content height
    scrollPosition_ = clamp(scrollPosition_ + 0.02f * -delta, 0.f, 1.f);
}

}
