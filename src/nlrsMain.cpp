#include "core/nlrsMouse.h"
#include "core/nlrsRepository.h"
#include "core/nlrsApp.h"
#include "gui/nlrsScreen.h"
#include "nlrsAllocator.h"
#include "nlrsLog.h"
#include "nlrsGraphicsApi.h"
#include "nlrsWindow.h"

#include "SDL_events.h"

#include <cstdlib>

using nlrs::usize;
using nlrs::u32;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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
        *       ___                    _ __                  _      _ __
        *      / _ \___ ___  ___  ___ (_) /____  ______ __  (_)__  (_) /_
        *     / , _/ -_) _ \/ _ \(_-</ / __/ _ \/ __/ // / / / _ \/ / __/
        *    /_/|_|\__/ .__/\___/___/_/\__/\___/_/  \_, / /_/_//_/_/\__/
        *            /_/                           /___/
        */
        nlrs::Repository repository("..");

        /***
        *       ___              __                  _      _ __
        *      / _ \___ ___  ___/ /__ _______ ____  (_)__  (_) /_
        *     / , _/ -_) _ \/ _  / -_) __/ -_) __/ / / _ \/ / __/
        *    /_/|_|\__/_//_/\_,_/\__/_/  \__/_/   /_/_//_/_/\__/
        *
        */
        nlrs::GraphicsApi graphicsApi;
        {
            nlrs::GraphicsApi::Options opts;

            if (!graphicsApi.initialize(opts))
            {
                LOG_ERROR << "Failed to initialize graphics api.";
                return -1;
            }
        }
        nlrs::GraphicsApiLocator::set(&graphicsApi);

        /***
        *       ________       __         _           _      _ __
        *      /  _/ __ \  ___/ /__ _  __(_)______   (_)__  (_) /_
        *     _/ // /_/ / / _  / -_) |/ / / __/ -_) / / _ \/ / __/
        *    /___/\____/  \_,_/\__/|___/_/\__/\__/ /_/_//_/_/\__/
        *
        */
        nlrs::Mouse mouse;
        nlrs::MouseLocator::set(&mouse);

        /***
        *       ___               _      _ __
        *      / _ | ___  ___    (_)__  (_) /_
        *     / __ |/ _ \/ _ \  / / _ \/ / __/
        *    /_/ |_/ .__/ .__/ /_/_//_/_/\__/
        *         /_/  /_/
        */
        nlrs::App app;
        if (!app.initialize())
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

        bool running = true;
        while (running)
        {
            mouse.update(); // updates mouse coordinates
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                mouse.handleEvent(event);
            }

            app.update();

            graphicsApi.clearBuffers();

            app.render();

            graphicsApi.swapBuffers();
        }
    }

    return 0;
}
