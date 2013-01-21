#ifndef MENUBAR_H
#define MENUBAR_H

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

#include "filemenu.h"
#include "MM.h"
#include "../include/log.h"

#define FILE_NAME "File"

#define MENUBAR_DEBUG

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);
    virtual ~MenuBar();
    
signals:
    void open_location_signal(QString &location);
    void open_file_signal(QString &filename);
    void create_new_tab_signal();
    void exit_signal();

public slots:

private slots:
#ifdef MENUBAR_DEBUG
    void exit_slot();
#endif

private:
    void init();
    void set_signal();
    void add_menu();

private:
    FileMenu *file_menu;
};

#endif // MENUBAR_H
