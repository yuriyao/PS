#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    init();
    set_signal();
    add_menu();
}

MenuBar::~MenuBar()
{
    file_menu->~FileMenu();
    Ps_Delete(file_menu);
}

void
MenuBar::init()
{
    /*Init file menu*/
    file_menu = Ps_New<FileMenu>();
    new(file_menu) FileMenu(0);
    file_menu->setTitle(QString(tr(FILE_NAME)));
}


void
MenuBar::set_signal()
{
    /*Pass The signal from file menu*/
    connect(file_menu, SIGNAL(open_location_signal(QString&)), this, SIGNAL(open_location_signal(QString&)));
    connect(file_menu, SIGNAL(open_file_signal(QString&)), this, SIGNAL(open_file_signal(QString&)));
    connect(file_menu, SIGNAL(create_new_tab_signal()), this, SIGNAL(create_new_tab_signal()));
    connect(file_menu, SIGNAL(exit_signal()), this, SIGNAL(exit_signal()));
#ifdef MENUBAR_DEBUG
    connect(this, SIGNAL(exit_signal()), this, SLOT(exit_slot()));
#endif
}

void
MenuBar::add_menu()
{
    this->addMenu(file_menu);
}

#ifdef MENUBAR_DEBUG
#include <iostream>
using namespace std;
void
MenuBar::exit_slot()
{
    cout << "Exit signal send" << endl;
    Ps_Log((char*)"Exit signal send\n", Ps_LOG_WARING);
}

#endif


