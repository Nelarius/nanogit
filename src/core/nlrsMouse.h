#pragma once

#include "nlrsArray.h"
#include "nlrsLocator.h"
#include "nlrsVector.h"
#include "SimpleSignal.h"

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
        Left = 1u,
        Middle,
        Right
    };

    enum class Event
    {
        ButtonDown,
        ButtonUp
    };

    // The callback gets called with the event button, and the event coordinates
    using ButtonCallback = std::function<void(Button, Vec2i)>;
    // The callback gets called the direction of the scroll, as well as the event coordinates.
    // A positive delta indicates scrolling away from the user, and a negative delta
    // scrolling towards the user.
    using ScrollCallback = std::function<void(i32, Vec2i)>;

    Mouse()
        : mouseState_(),
        coordinates_(0, 0),
        buttonDownSignal_(),
        buttonUpSignal_(),
        scrollSignal_()
    {
        mouseState_.pushBack(ButtonState{ Button::Left, Event::ButtonUp });
        mouseState_.pushBack(ButtonState{ Button::Middle, Event::ButtonUp });
        mouseState_.pushBack(ButtonState{ Button::Right, Event::ButtonUp });
    }

    Vec2i currentCoordinates() const;

    void listenToButtonDown(ButtonCallback callback);
    void listenToButtonUp(ButtonCallback callback);
    void listenToScroll(ScrollCallback callback);

    void update();

    bool handleEvent(const SDL_Event& event);

private:
    struct ButtonState
    {
        Button button;
        Event status;
    };

    StaticArray<ButtonState, 3> mouseState_;
    Vec2i coordinates_;

    Simple::Signal<void(Button, Vec2i)> buttonDownSignal_;
    Simple::Signal<void(Button, Vec2i)> buttonUpSignal_;
    Simple::Signal<void(i32, Vec2i)> scrollSignal_;
};

using MouseLocator = Locator<Mouse>;

}
