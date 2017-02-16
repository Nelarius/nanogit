#include "nlrsWidget.h"
#include "nlrsLog.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    child_(nullptr),
    margin_(0),
    border_(0),
    padding_(0)
{
    if (parent)
    {
        context_ = parent->context();
    }
}

Widget::~Widget()
{
    // TODO: use smart pointer here
    freeChild();
}

void Widget::onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates)
{
    if (contentBounds().contains(coordinates))
    {
        if (child_)
        {
            child_->onMouseButton(button, event, coordinates);
        }
    }
}

void Widget::onMouseScroll(i32 delta, Vec2i coordinates)
{
    if (contentBounds().contains(coordinates))
    {
        if (child_)
        {
            child_->onMouseScroll(delta, coordinates);
        }
    }
}

void Widget::setMargin(int margin)
{
    if (margin < 0)
    {
        LOG_ERROR << "Attempted to set negative margin";
        return;
    }

    margin_ = margin;
}

void Widget::setBorder(int border)
{
    if (border < 0)
    {
        LOG_ERROR << "Attempted to set negative border";
        return;
    }

    border_ = border;
}

void Widget::setPadding(int padding)
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

Bounds2i Widget::contentBounds() const
{
    // TODO: validate the calculated size
    auto bounds = parent_->contentBounds();
    return bounds.shrink(i32(margin_ + border_ + padding_));
}

void Widget::freeChild()
{
    if (child_)
    {
        child_->~Widget();
        allocator_.free(child_);
    }
}

}
