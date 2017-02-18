#pragma once

#include "nlrsGeometry.h"
#include "nlrsMemory.h"
#include "core/nlrsMouse.h"

#include <utility>

struct NVGcontext;

namespace nlrs
{

class Widget
{
public:
    Widget(Widget* parent, IAllocator& allocator);
    virtual ~Widget() = default;

    virtual void onRender() = 0;
    virtual void onMouseButton(Mouse::Button button, Mouse::Event event, Vec2i coordinates);
    virtual void onMouseScroll(i32 delta, Vec2i coordinates);
    virtual void onMouseOver(Vec2i coordinates);
    // the default implementation just returns the parent's context
    virtual NVGcontext* context();

    // the screen space bounds of the widget content
    // the default implementation offsets the parent's content bounds
    // by this widget's margin, padding, and border
    virtual Bounds2f contentBounds() const;

    // Construct and add a child using the default Widget constructor
    template<typename T>
    T& addChild()
    {
        child_ = makeScopedPtrForBaseClass<Widget, T>(allocator_, this, allocator_);
        return *dynamic_cast<T*>(child_.get());
    }

    template<typename T>
    T& addChild(ScopedPtr<Widget>&& child)
    {
        child_ = std::move(child);
        return *dynamic_cast<T*>(child_.get());
    }

    void setMargin(float margin);
    void setBorder(float border);
    void setPadding(float padding);

protected:

    IAllocator& allocator_;
    Widget* parent_;
    ScopedPtr<Widget> child_;
    NVGcontext* context_;

    // element information
    float margin_;
    float border_;
    float padding_;
};

}
