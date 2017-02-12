#pragma once

#include "nlrsArray.h"
#include "nlrsGeometry.h"
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
    // the default implementation offsets the parent's size
    virtual Bounds2i size() const;

    void addChild(Widget* child);
    void setMargin(int margin);
    void setBorder(int border);
    void setPadding(int padding);

protected:
    void freeChildren_();

    IAllocator& allocator_;
    Widget* parent_;
    NVGcontext* context_;
    StaticArray<Widget*, 8u> children_;

    // element information
    int margin_;
    int border_;
    int padding_;
};

}
