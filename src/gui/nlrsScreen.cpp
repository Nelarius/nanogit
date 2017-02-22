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
{
    position_ = Vec2f(0.f, 0.f);
}

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
    size_ = windowSize.cast<float>();

#ifdef NLRS_DEBUG
    context_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    context_ = nvgCreateGLw3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
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

void Screen::onRender()
{
    NLRS_ASSERT(context_);

    // TODO: calculate pixel ratio
    float pixelRatio = 1.f;

    nvgSave(context_);
    {
        nvgBeginFrame(context_, int(size_.x), int(size_.y), pixelRatio);

        child_->onRender();

        nvgEndFrame(context_);
    }
    nvgRestore(context_);
}

void Screen::onResize(Vec2i newSize)
{
    NLRS_ASSERT(context_);

    size_ = newSize.cast<float>();
}

}
