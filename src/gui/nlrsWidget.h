#pragma once

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
    virtual void onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates);
    virtual void onMouseScroll(i32 delta, Vec2i coordinates);
    // the default implementation just returns the parent's context
    virtual NVGcontext* context();

    // the screen space bounds of the widget content
    // the default implementation offsets the parent's content bounds
    // by this widget's margin, padding, and border
    virtual Bounds2i contentBounds() const;

    template<typename T>
    T* addChild()
    {
        void* mem = allocator_.allocate(sizeof(T), sizeof(T));
        T* widget = new (mem) T(static_cast<Widget*>(this), allocator_);

        child_ = widget;

        return widget;
    }

    void setMargin(int margin);
    void setBorder(int border);
    void setPadding(int padding);

protected:
    void freeChild();

    IAllocator& allocator_;
    Widget* parent_;
    Widget* child_;
    NVGcontext* context_;

    // element information
    int margin_;
    int border_;
    int padding_;
};

}
