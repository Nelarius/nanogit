#include "nlrsWidget.h"
#include "nlrsLog.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    child_(allocator)
{
    if (parent)
    {
        context_ = parent->context();
    }
}

void Widget::onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseButton(button, event, coordinates);
        }
    }
}

void Widget::onMouseScroll(i32 delta, Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseScroll(delta, coordinates);
        }
    }
}

void Widget::onMouseOver(Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        if (child_)
        {
            child_->onMouseOver(coordinates);
        }
    }
}

NVGcontext* Widget::context()
{
    return parent_->context();
}

}
