#pragma once

#include "nlrsWidget.h"
#include "nlrsText.h"

#include <functional>
#include <string>

namespace nlrs
{

class Button : public Widget
{
public:
    Button(Widget* parent, IAllocator& allocator)
        : Widget(parent, allocator),
        caption_(),
        cornerRadius_(0.f),
        pushed_(false),
        topGradientPushed_(0, 0, 0),
        bottomGradientPushed_(255, 255, 255),
        topGradientUnfocused_(255, 255, 255),
        bottomGradientUnfocused_(0, 0, 0),
        callback_()
    {}

    virtual ~Button() = default;

    void onRender() override;

    // calls the button's callback function here
    // does not pass the event forward to children
    void onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates) override;

    inline void setText(const std::string& text)
    {
        caption_.text = text;
    }

    inline void setFont(FontManager::Handle fontId)
    {
        if (fontId != FontManager::InvalidHandle)
        {
            caption_.fontId = fontId;
        }
    }

    inline void setFontSize(float size)
    {
        NLRS_ASSERT(size >= 0.f);
        caption_.fontSize = size;
    }

    inline void setCornerRadius(float radius)
    {
        NLRS_ASSERT(radius >= 0.f);
        cornerRadius_ = radius;
    }

    inline void setUnfocusedGradient(const Vector3<u8>& top, const Vector3<u8>& bottom)
    {
        topGradientUnfocused_ = top;
        bottomGradientUnfocused_ = bottom;
    }

    inline void setPushedGradient(const Vector3<u8>& top, const Vector3<u8>& bottom)
    {
        topGradientPushed_ = top;
        bottomGradientPushed_ = bottom;
    }

    inline void setCallback(std::function<void()> callback)
    {
        callback_ = callback;
    }

private:
    Text caption_;
    float cornerRadius_;
    bool pushed_;
    Vector3<u8> topGradientPushed_;
    Vector3<u8> bottomGradientPushed_;
    Vector3<u8> topGradientUnfocused_;
    Vector3<u8> bottomGradientUnfocused_;

    std::function<void()> callback_;
};

}
