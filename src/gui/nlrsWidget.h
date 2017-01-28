#pragma once

#include "nlrsArray.h"
#include "core/nlrsMouse.h"

struct NVGcontext;

namespace nlrs
{

class Widget
{
public:
    Widget(Widget* parent, IAllocator& allocator);
    virtual ~Widget();

    virtual void onRender() = 0;
    // the default implementation just returns the parent's context
    virtual NVGcontext* context();

    void addChild(Widget* child);

protected:
    void freeChildren_();

    IAllocator& allocator_;
    Widget* parent_;
    NVGcontext* context_;
    StaticArray<Widget*, 16u> children_;
};

}
