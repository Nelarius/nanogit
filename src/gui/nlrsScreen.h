#pragma once

#include "nlrsVector.h"
#include "nlrsWidget.h"

struct NVGcontext;

namespace nlrs
{

class Screen : public Widget
{
public:
    Screen();
    ~Screen();

    bool initialize(Vec2i windowSize); // TODO: this should return false on failure to initialize nanovg

    void onRender() override;

    void onResize(Vec2i newSize);

private:
    NVGcontext* context_;
    Vec2i size_;
};

}
