#include "nlrsMouse.h"

#include "SDL_events.h"

namespace nlrs
{

Vec2i Mouse::currentCoordinates() const
{
    Vec2i coords;
    SDL_GetMouseState(&coords.x, &coords.y);
    return coords;
}

void Mouse::listenToButtonDown(ButtonCallback callback)
{
    buttonDownSignal_.connect(callback);
}

void Mouse::listenToButtonUp(ButtonCallback callback)
{
    buttonUpSignal_.connect(callback);
}

void Mouse::listenToScroll(ScrollCallback callback)
{
    scrollSignal_.connect(callback);
}

bool Mouse::handleEvent(const SDL_Event& event)
{
    static_assert(static_cast<int>(Button::Left) == SDL_BUTTON_LEFT, "Button::Left not equal to SDL_BUTTON_LEFT");
    static_assert(static_cast<int>(Button::Middle) == SDL_BUTTON_MIDDLE, "Button::Middle not equal to SDL_BUTTON_MIDDLE");
    static_assert(static_cast<int>(Button::Right) == SDL_BUTTON_RIGHT, "Button::Right not equal to SDL_BUTTON_RIGHT");

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        buttonDownSignal_.emit(static_cast<Button>(event.button.button), coordinates_);
        return true;
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        buttonUpSignal_.emit(static_cast<Button>(event.button.button), coordinates_);
        return true;
    }
    else if (event.type == SDL_MOUSEWHEEL)
    {
        scrollSignal_.emit(event.wheel.y, coordinates_);
        return true;
    }

    return false;
}

void Mouse::update()
{
    SDL_GetMouseState(&coordinates_.x, &coordinates_.y);
}

}
