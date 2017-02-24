#include "nlrsWidget.h"
#include "nlrsLog.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    children_(allocator)
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
        for (auto& child : children_)
        {
            child->onMouseButton(button, event, coordinates);
        }
    }
}

void Widget::onMouseScroll(i32 delta, Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        for (auto& child : children_)
        {
            child->onMouseScroll(delta, coordinates);
        }
    }
}

void Widget::onMouseOver(Vec2i coordinates)
{
    if (Bounds2f(position_, position_ + size_).contains(coordinates.cast<float>()))
    {
        for (auto& child : children_)
        {
            child->onMouseOver(coordinates);
        }
    }
}

NVGcontext* Widget::context()
{
    return parent_->context();
}

}
