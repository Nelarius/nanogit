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

    // the screen space bounds
    // the default implementation offsets the parent's bounds according to the
    // margin & padding
    virtual Bounds2i bounds() const;

    template<typename T>
    T* addChild()
    {
        void* mem = allocator_.allocate(sizeof(T), sizeof(T));
        T* widget = new (mem) T(static_cast<Widget*>(this), allocator_);

        children_.pushBack(widget);

        return widget;
    }

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
