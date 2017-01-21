#pragma once

#include "nlrsArray.h"

namespace nlrs
{

class Widget
{
public:
    Widget(Widget* parent)
        : parent_(parent),
        children_()
    {}

    virtual ~Widget();

    virtual void onRender() = 0;

    void addChild(Widget* child);

protected:
    Widget* parent_;
    StaticArray<Widget*, 16u> children_;
};

}
