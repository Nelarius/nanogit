#include "nlrsApp.h"
#include "nlrsAllocator.h"
#include "nlrsWindow.h"
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

    void* mem = allocator.allocate(sizeof(TextBox), alignof(TextBox));
    auto* textBox = new (mem) TextBox(&screen_, allocator);

    screen_.addChild(textBox);

    textBox->setFont(handle);
    textBox->setFontSize(18.f);
    textBox->setMargin(50);
    textBox->setText(repository_.diffIndexToWorkDir().c_str());

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
