#include "nlrsWidget.h"

namespace nlrs
{

Widget::~Widget()
{}

void Widget::addChild(Widget* child)
{
    children_.pushBack(child);
}

}
