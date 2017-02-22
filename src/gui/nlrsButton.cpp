#include "nlrsButton.h"
#include "nanovg.h"

namespace nlrs
{

void Button::onRender()
{
    NVGcolor gradientTop;
    NVGcolor gradientBot;
    if (pushed_)
    {
        const Vector3<u8>& ct = topGradientPushed_;
        const Vector3<u8>& cb = bottomGradientPushed_;
        gradientTop = nvgRGBA(ct.r, ct.g, ct.b, 32);
        gradientBot = nvgRGBA(cb.r, cb.g, cb.b, 32);
    }
    else
    {
        const Vector3<u8>& ct = topGradientUnfocused_;
        const Vector3<u8>& cb = bottomGradientUnfocused_;
        gradientTop = nvgRGBA(ct.r, ct.g, ct.b, 32);
        gradientBot = nvgRGBA(cb.r, cb.g, cb.b, 32);
    }

    nvgBeginPath(context_);

    // draw button gradient
    nvgRoundedRect(context_, position_.x + 1.f, position_.y + 1.f, size_.x - 2.f, size_.y - 2.f, cornerRadius_ - 1.f);
    NVGpaint bg = nvgLinearGradient(
        context_, position_.x, position_.y, position_.x, position_.y + size_.y, gradientTop, gradientBot);
    nvgFillPaint(context_, bg);
    nvgFill(context_);

    // draw button outline
    nvgBeginPath(context_);
    nvgStrokeWidth(context_, 1.f);
    // when pushed, the outline is 1 pixel lower
    nvgRoundedRect(
        context_, position_.x + 0.5f, position_.y + (pushed_ ? 0.5f : 1.5f), size_.x - 1.f, size_.y - 1.f - (pushed_ ? 0.f : 1.f), cornerRadius_);
    nvgStrokeColor(context_, nvgRGB(0u, 0u, 0u));
    nvgStroke(context_);

    // TODO
    // render an optional icon

    // render the caption
    float tw = nvgTextBounds(context_, 0.f, 0.f, caption_.text.c_str(), nullptr, nullptr);
    nvgFontSize(context_, caption_.fontSize);
    nvgFontFaceId(context_, caption_.fontId);
    nvgTextAlign(context_, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(context_, nvgRGBA(0, 0, 0, 160));
    nvgText(context_, position_.x + size_.x*0.5f - tw*0.5f, position_.y + size_.y*0.5f - 1.f, caption_.text.c_str(), NULL);
    nvgFillColor(context_, nvgRGBA(255, 255, 255, 160));
    nvgText(context_, position_.x + size_.x*0.5f - tw*0.5f, position_.y + size_.y*0.5f, caption_.text.c_str(), NULL);
}

void Button::onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        if (button == Mouse::Button::Left && event == Mouse::Event::ButtonDown)
        {
            pushed_ = true;
        }
        else if (button == Mouse::Button::Left && event == Mouse::Event::ButtonUp)
        {
            callback_();
            pushed_ = false;
        }
    }
}

}
