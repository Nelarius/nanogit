#include "nlrsWidget.h"
#include "nlrsLog.h"
#include "nanovg.h"

namespace nlrs
{

Widget::Widget(Widget* parent, IAllocator& allocator)
    : allocator_(allocator),
    parent_(parent),
    children_(),
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
    freeChildren_();
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

Bounds2i Widget::bounds() const
{
    // TODO: validate the calculated size
    auto bounds = parent_->bounds();
    return Bounds2i({bounds.min.x + margin_, bounds.min.y + margin_}, {bounds.max.x - margin_, bounds.max.y - margin_});
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
