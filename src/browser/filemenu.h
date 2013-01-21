/**
 * File Menu
 * SHIT QT5,Why cannot input Chinese....
 **/
#ifndef FILEMENU_H
#define FILEMENU_H

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QKeySequence>
#include <QtWidgets/QFileDialog>
#include "MM.h"
#include "../include/config.h"


#define CREATE_NEW_TAB "New Tab"
#define OPEN_LOCATION "Open Location"
#define OPEN_FILE "Open File"
#define EXIT "Exit"

#define CREATE_NEW_TAB_SHORT "Ctrl+N"
#define OPEN_LOCATION_SHORT "Ctrl+L"
#define OPEN_FILE_SHORT "Ctrl+O"
#define EXIT_SHORT "Ctrl+Q"

class FileChooser : public QFileDialog
{
    Q_OBJECT
public:
    FileChooser(QWidget *parent = 0, QString *dir = 0);
    void set_dir(const QString &dir);
    QString& get_path();
private:
    QString path;
};

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileMenu(QWidget *parent = 0);
    virtual ~FileMenu();
signals:
    void open_location_signal(QString &location);
    void open_file_signal(QString &filename);
    void create_new_tab_signal();
    void exit_signal();

public slots:


private slots:
    void open_file_slot();

private:
    void init_items();
    void patch_signal();
    void add_action();

private:
    QAction *new_tab_item;
    QAction *open_location_item;
    QAction *open_file_item;
    QAction *exit_item;
    QString last_open_dir;
};

#endif // FILEMENU_H
