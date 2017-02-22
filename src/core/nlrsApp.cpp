#include "nlrsAllocator.h"
#include "nlrsApp.h"
#include "nlrsLog.h"
#include "nlrsMouse.h"
#include "nlrsWindow.h"
#include "gui/nlrsButton.h"
#include "gui/nlrsScrollPanel.h"
#include "gui/nlrsTextBox.h"

#include <utility>

namespace nlrs
{

// TODO: figure a better scheme for alloting stuff
App::App()
    : screen_(SystemAllocator::getInstance()),
    fontManager_(nullptr),
    font_(FontManager::InvalidHandle),
    repository_("..")
{}

App::App(App&& other)
    : screen_(std::move(other.screen_)),
    fontManager_(std::move(other.fontManager_)),
    font_(other.font_),
    repository_(std::move(other.repository_))
{}

bool App::initialize()
{
    screen_.initialize(WindowLocator::get()->size());
    fontManager_.setContext(screen_.context());

    auto handle = fontManager_.load("sans", "Roboto-Regular.ttf");

    if (handle == FontManager::InvalidHandle)
    {
        return false;
    }

    IAllocator& allocator = SystemAllocator::getInstance();

    ScrollPanel& scrollPanel = screen_.addChild<ScrollPanel>();
    scrollPanel.setRadius(5.f);
    scrollPanel.setFeather(5.f);
    scrollPanel.setScrollBarWidth(16);
    scrollPanel.setPosition(Vec2f(0.f, 0.f));
    auto& size = screen_.size();
    scrollPanel.setSize(Vec2f(size.x, size.y - 50.f));

    TextBox& textBox = scrollPanel.addChild<TextBox>();
    textBox.setFont(handle);
    textBox.setFontSize(18.f);
    textBox.setText(repository_.diffIndexToWorkDir().c_str());

    /*Button& button = screen_.addChild<Button>();
    float buttonWidth = 100.f;
    button.setPosition(Vec2f(0.5f * (size.x - buttonWidth), size.y - 50.f));
    button.setSize(Vec2f(buttonWidth, 50));
    button.setText("button");
    button.setFont(handle);
    button.setFontSize(24.f);
    button.setCallback([]() -> void { LOG_INFO << "The button was pressed!"; });*/

    Mouse& mouse = *MouseLocator::get();
    mouse.listenToButtonDown([this](Mouse::Button button, Vec2i coords) -> void {
        screen_.onMouseButton(button, Mouse::Event::ButtonDown, coords);
    });
    mouse.listenToButtonUp([this](Mouse::Button button, Vec2i coords) -> void {
        screen_.onMouseButton(button, Mouse::Event::ButtonUp, coords);
    });
    mouse.listenToScroll([this](i32 delta, Vec2i coords) -> void {
        screen_.onMouseScroll(delta, coords);
    });

    return true;
}

void App::update()
{
    //
}

void App::render()
{
    screen_.onRender();
}

}
