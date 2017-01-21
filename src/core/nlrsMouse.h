#pragma once

#include "nlrsLocator.h"
#include "nlrsVector.h"

#include <functional>

union SDL_Event;

namespace nlrs
{

class Mouse
{
public:
    /*
    * These enumerations correspond to SDL_BUTTON_* values,
    * defined in http://hg.libsdl.org/SDL/file/default/include/SDL_mouse.h
    */
    enum class Button : int {
        Left = 1,
        Middle,
        Right
    };

    enum class Event
    {
        ButtonDown,
        ButtonUp
    };

    using Callback = std::function<void(Button button)>;

    Vec2i currentCoordinates() const;

    bool handleEvent(const SDL_Event& event);

private:
};

using MouseLocator = Locator<Mouse>;

}
