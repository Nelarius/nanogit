#include "nlrsWidget.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    children_()
{
    if (parent)
    {
        context_ = parent->context();
    }
}

Widget::~Widget()
{
    freeChildren_();
}

void Widget::addChild(Widget* child)
{
    children_.pushBack(child);
}

NVGcontext* Widget::context()
{
    return parent_->context();
}

void Widget::freeChildren_()
{
    for (Widget* widget : children_)
    {
        widget->~Widget();
        allocator_.free(widget);
    }
}

}
