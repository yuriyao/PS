#ifndef BROWSER_H
#define BROWSER_H
#include <QtWidgets/QMainWindow>

#include "menubar.h"
#include "toolbar.h"
#include "MM.h"
#include "../include/log.h"

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    Browser();
    virtual ~Browser();

private slots:
    void clear();

private:
    void init();
    void add_widgets();
    void signal_deal();

private:
    MenuBar *menu_bar;
    ToolBar *tool_bar;
};

#endif // BROWSER_H
