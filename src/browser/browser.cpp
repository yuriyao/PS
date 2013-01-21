#include "browser.h"

Browser::Browser() :
    QMainWindow()
{
    init();
    add_widgets();
    signal_deal();
}

Browser::~Browser()
{
    menu_bar->~MenuBar();
    Ps_Delete(menu_bar);
    tool_bar->~ToolBar();
    Ps_Delete(tool_bar);
}

void
Browser::clear()
{
    this->close();
}

void
Browser::init()
{
    menu_bar = Ps_New<MenuBar>();
    new(menu_bar) MenuBar(0);
    tool_bar = Ps_New<ToolBar>();
    new(tool_bar) ToolBar(this);
}

void
Browser::add_widgets()
{
    this->setMenuBar(menu_bar);
    this->addToolBar(tool_bar);
}

void
Browser::signal_deal()
{
    connect(menu_bar, SIGNAL(exit_signal()), this, SLOT(clear()));

}
