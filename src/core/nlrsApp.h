#pragma once

#include "gui/nlrsFontManager.h"
#include "gui/nlrsScreen.h"
#include "nlrsRepository.h"

namespace nlrs
{

class App
{
public:
    App();
    App(App&&);
    ~App() = default;

    App(const App&) = delete;
    App& operator=(App&&) = delete;
    App& operator=(const App&) = delete;

    bool initialize();
    void update();
    void render();

private:
    Screen screen_;
    FontManager fontManager_;
    FontManager::Handle font_;
    Repository repository_;
};

}
