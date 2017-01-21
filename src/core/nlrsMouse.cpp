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

bool Mouse::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        // TODO:
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONUP)
    {
        // TODO:
        return true;
    }
    return false;
}

}
