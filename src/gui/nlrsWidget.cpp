#include "nlrsWidget.h"
#include "nlrsLog.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    child_(allocator),
    margin_(0.f),
    border_(0.f),
    padding_(0.f)
{
    if (parent)
    {
        context_ = parent->context();
    }
}

void Widget::onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates)
{
    if (contentBounds().contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseButton(button, event, coordinates);
        }
    }
}

void Widget::onMouseScroll(i32 delta, Vec2i coordinates)
{
    if (contentBounds().contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseScroll(delta, coordinates);
        }
    }
}

void Widget::onMouseOver(Vec2i coordinates)
{
    if (contentBounds().contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseOver(coordinates);
        }
    }
}

void Widget::setMargin(float margin)
{
    if (margin < 0)
    {
        LOG_ERROR << "Attempted to set negative margin";
        return;
    }

    margin_ = margin;
}

void Widget::setBorder(float border)
{
    if (border < 0)
    {
        LOG_ERROR << "Attempted to set negative border";
        return;
    }

    border_ = border;
}

void Widget::setPadding(float padding)
{
    if (padding < 0)
    {
        LOG_ERROR << "Attempted to set negative padding";
        return;
    }
    padding_ = padding;
}

NVGcontext* Widget::context()
{
    return parent_->context();
}

Bounds2f Widget::contentBounds() const
{
    // TODO: validate the calculated size
    auto bounds = parent_->contentBounds();
    return bounds.shrink(margin_ + border_ + padding_);
}


}
