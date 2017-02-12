#pragma once

#include "nlrsAllocator.h"
#include "nlrsVector.h"
#include "nlrsWidget.h"

struct NVGcontext;

namespace nlrs
{

class Screen : public Widget
{
public:
    Screen() = default;
    explicit Screen(IAllocator& allocator);
    Screen(Screen&&);
    virtual ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(Screen&&) = delete;
    Screen& operator=(const Screen&) = delete;

    bool initialize(Vec2i windowSize);
    void onResize(Vec2i newSize);

    void onRender() override;
    NVGcontext* context() override;
    Bounds2i size() const override;

private:
    NVGcontext* context_;
    Vec2i size_;
};

}
