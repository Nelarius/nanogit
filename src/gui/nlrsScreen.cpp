#include "nlrsScreen.h"

#include "GL/gl3w.h"
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

void drawEditBoxBase(NVGcontext* vg, float x, float y, float w, float h)
{
    NVGpaint bg;
    // Edit
    bg = nvgBoxGradient(vg, x + 1, y + 1 + 1.5f, w - 2, h - 2, 3, 4, nvgRGBA(255, 255, 255, 32), nvgRGBA(32, 32, 32, 32));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, 4 - 1);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 4 - 0.5f);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
    nvgStroke(vg);
}

void drawEditBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{

    drawEditBoxBase(vg, x, y, w, h);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(vg, x + h*0.3f, y + h*0.5f, text, NULL);
}

void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
    float cornerRadius = 3.0f;
    NVGpaint shadowPaint;
    NVGpaint headerPaint;

    nvgSave(vg);
    //	nvgClearState(vg);

    // Window
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, cornerRadius);
    nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
    //	nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    // Drop shadow
    shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
    nvgBeginPath(vg);
    nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
    nvgRoundedRect(vg, x, y, w, h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    // Header
    headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 1, y + 1, w - 2, 30, cornerRadius - 1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
    nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg, 2);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
    nvgText(vg, x + w / 2, y + 16 + 1, title, NULL);

    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
    nvgText(vg, x + w / 2, y + 16, title, NULL);

    nvgRestore(vg);
}

namespace nlrs
{

Screen::Screen()
    : Widget(nullptr),
    context_(nullptr)
{}

Screen::~Screen()
{
    nvgDeleteGL3(context_);
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

void Screen::onRender()
{
    nvgBeginFrame(context_, size_.x, size_.y, float(size_.x) / size_.y);

    for (auto child : children_)
    {
        child->onRender();
    }

    drawWindow(context_, "Widgets `n Stuff", 50, 50, 300, 400);
    drawEditBox(context_, "Email", 50, 50, 280, 28);
    drawEditBox(context_, "Password", 50, 85, 280, 28);
    nvgRestore(context_);

    nvgEndFrame(context_);
}

void Screen::onResize(Vec2i newSize)
{
    size_ = newSize;
}

}
