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

    // Construct and add a child using the default Widget constructor
    template<typename T>
    T& addChild()
    {
        auto indx = children_.emplaceBack(makeScopedPtrForBaseClass<Widget, T>(allocator_, this, allocator_));
        return *dynamic_cast<T*>(children_[indx].get());
    }

    template<typename T>
    T& addChild(ScopedPtr<Widget>&& child)
    {
        auto indx = children_.emplaceBack(std::move(child));
        return *dynamic_cast<T*>(children_[indx].get());
    }

    inline Vec2f position() const { return position_; }
    inline void setPosition(Vec2f pos) { NLRS_ASSERT(pos.x >= 0.f && pos.y >= 0.f); position_ = pos;}

    inline Vec2f size() const { return size_; }
    inline void setSize(Vec2f size) { NLRS_ASSERT(size.x >= 0.f && size.y >= 0.f); size_ = size; }

protected:
    IAllocator& allocator_;
    Widget* parent_;
    Array<ScopedPtr<Widget>> children_;
    NVGcontext* context_;

    // element information
    Vec2f position_; // the position relative to the parent
    Vec2f size_; // the extents of the widget
};

}
