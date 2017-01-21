#include "nlrsAllocator.h"
#include "nlrsLog.h"
#include "gui/nlrsScreen.h"
#include "core/nlrsMouse.h"
#include "nlrsRenderer.h"
#include "nlrsWindow.h"

#include "SDL_events.h"

#include <cstdlib>

using nlrs::usize;
using nlrs::u32;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

int main(int argc, char** argv)
{
    /***
    *       _______  __       _      ___         __             _      _ __
    *      / __/ _ \/ /      | | /| / (_)__  ___/ /__ _    __  (_)__  (_) /_
    *     _\ \/ // / /___    | |/ |/ / / _ \/ _  / _ \ |/|/ / / / _ \/ / __/
    *    /___/____/____( )   |__/|__/_/_//_/\_,_/\___/__,__/ /_/_//_/_/\__/
    *                  |/
    */
    nlrs::SdlWindow window;
    {
        nlrs::SdlWindow::Options opts;
        opts.name = "nanogit";
        opts.width = WINDOW_WIDTH;
        opts.height = WINDOW_HEIGHT;

        if (!window.initialize(opts))
        {
            LOG_ERROR << "Failed to initialize window.";
            return -1;
        }
    }
    nlrs::WindowLocator::set(&window);

    const usize heapSize = 10u * 1024 * 1024u;
    void* arena = std::malloc(heapSize);

    {
        /***
        *       ___   ____              __             _      _ __
        *      / _ | / / /__  _______ _/ /____  ____  (_)__  (_) /_
        *     / __ |/ / / _ \/ __/ _ `/ __/ _ \/ __/ / / _ \/ / __/
        *    /_/ |_/_/_/\___/\__/\_,_/\__/\___/_/   /_/_//_/_/\__/
        *
        */
        nlrs::FreeListAllocator heapAllocator(arena, heapSize);

        nlrs::HeapAllocatorLocator::set(&heapAllocator);
        nlrs::SystemAllocatorLocator::set(&nlrs::SystemAllocator::getInstance());
        NLRS_ASSERT(nlrs::HeapAllocatorLocator::get() != nlrs::SystemAllocatorLocator::get());

        /***
        *       ___              __                  _      _ __
        *      / _ \___ ___  ___/ /__ _______ ____  (_)__  (_) /_
        *     / , _/ -_) _ \/ _  / -_) __/ -_) __/ / / _ \/ / __/
        *    /_/|_|\__/_//_/\_,_/\__/_/  \__/_/   /_/_//_/_/\__/
        *
        */
        nlrs::Renderer renderer;
        {
            nlrs::Renderer::Options opts;

            if (!renderer.initialize(opts))
            {
                LOG_ERROR << "Failed to initialize renderer.";
                return -1;
            }
        }
        nlrs::RendererLocator::set(&renderer);

        /***
        *      _______  ______  _      _ __
        *     / ___/ / / /  _/ (_)__  (_) /_
        *    / (_ / /_/ // /  / / _ \/ / __/
        *    \___/\____/___/ /_/_//_/_/\__/
        *
        */
        nlrs::Screen screen;
        if (!screen.initialize(nlrs::Vec2i(WINDOW_WIDTH, WINDOW_HEIGHT)))
        {
            return -1;
        }

        /***
        *       __  ___     _        __
        *      /  |/  /__ _(_)__    / /__  ___  ___
        *     / /|_/ / _ `/ / _ \  / / _ \/ _ \/ _ \
        *    /_/  /_/\_,_/_/_//_/ /_/\___/\___/ .__/
        *                                    /_/
        */
        nlrs::Mouse mouse;
        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                mouse.handleEvent(event);
            }

            screen.onRender();

            renderer.swapBuffers();
        }
    }

    return 0;
}
