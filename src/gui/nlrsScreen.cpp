#include "nlrsScreen.h"

#include "GL/gl3w.h"
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#undef max
#undef min
#undef near
#undef far

namespace nlrs
{

Screen::Screen(IAllocator& allocator)
    : Widget(nullptr, allocator),
    context_(nullptr)
{}

Screen::~Screen()
{
    if (context_)
    {
        nvgDeleteGL3(context_);
    }
}

Screen::Screen(Screen&& other)
    : Widget(nullptr, other.allocator_),
    context_(other.context_)
{
    other.context_ = nullptr;
}

bool Screen::initialize(Vec2i windowSize)
{
    size_ = windowSize;
    context_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if (!context_)
    {
        return false;
    }

    return true;
}

NVGcontext* Screen::context()
{
    NLRS_ASSERT(context_);
    return context_;
}

Bounds2i Screen::size() const
{
    return Bounds2i({0, 0}, {size_.x, size_.y});
}

void Screen::onRender()
{
    NLRS_ASSERT(context_);

    nvgBeginFrame(context_, size_.x, size_.y, float(size_.x) / size_.y);

    for (auto child : children_)
    {
        child->onRender();
    }

    nvgEndFrame(context_);
}

void Screen::onResize(Vec2i newSize)
{
    NLRS_ASSERT(context_);

    size_ = newSize;
}


}
