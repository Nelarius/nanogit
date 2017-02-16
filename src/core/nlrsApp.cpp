#include "nlrsAllocator.h"
#include "nlrsApp.h"
#include "nlrsMouse.h"
#include "nlrsWindow.h"
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

    screen_.setPadding(50);
    ScrollPanel* scrollPanel = screen_.addChild<ScrollPanel>();
    scrollPanel->setRadius(5.f);
    scrollPanel->setFeather(5.f);
    scrollPanel->setScrollBarWidth(16);
    TextBox* textBox = scrollPanel->addChild<TextBox>();

    textBox->setFont(handle);
    textBox->setFontSize(18.f);
    textBox->setText(repository_.diffIndexToWorkDir().c_str());

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
