#include "filemenu.h"

FileMenu::FileMenu(QWidget *parent) :
    QMenu(parent)
{
    init_items();
    add_action();
    patch_signal();
}

FileMenu::~FileMenu()
{
    new_tab_item->~QAction();
    Ps_Delete(new_tab_item);
    open_location_item->~QAction();
    Ps_Delete(open_location_item);
    open_file_item->~QAction();
    Ps_Delete(open_file_item);
    exit_item->~QAction();
    Ps_Delete(exit_item);
}

void
FileMenu::init_items()
{
    /**INIT new tab item***/
    new_tab_item = Ps_New<QAction>();
    new(new_tab_item) QAction(NULL);
    //new_tab_item = new QAction();
    new_tab_item->setText(QString(tr(CREATE_NEW_TAB)));
    new_tab_item->setShortcut(QKeySequence(tr(CREATE_NEW_TAB_SHORT)));
    /*Init open location item*/
    open_location_item = Ps_New<QAction>();
    new(open_location_item) QAction(NULL);
    //open_location_item = new QAction(NULL);
    open_location_item->setText(QString(tr(OPEN_LOCATION)));
    open_location_item->setShortcut(QKeySequence(tr(OPEN_LOCATION_SHORT)));
    /*Init open file item*/
    open_file_item = Ps_New<QAction>();
    new(open_file_item) QAction(NULL);
    //open_file_item = new QAction(NULL);
    open_file_item->setText(QString(tr(OPEN_FILE)));
    open_file_item->setShortcut(tr(OPEN_FILE_SHORT));
    /*Init exit item*/
    exit_item = Ps_New<QAction>();
    new(exit_item) QAction(NULL);
    //exit_item = new QAction(NULL);
    exit_item->setText(QString(tr(EXIT)));
    exit_item->setShortcut(tr(EXIT_SHORT));
}

void
FileMenu::patch_signal()
{
    connect(new_tab_item, SIGNAL(triggered()), this, SIGNAL(create_new_tab_signal()));
    connect(exit_item, SIGNAL(triggered()), this, SIGNAL(exit_signal()));
    connect(open_file_item, SIGNAL(triggered()), this, SLOT(open_file_slot()));
}

void
FileMenu::add_action()
{
    this->addAction(new_tab_item);
    this->addAction(open_location_item);
    this->addAction(open_file_item);
    this->addAction(exit_item);
}

void
FileMenu::open_file_slot()
{
    FileChooser chooser;
    QString filename;
    int pos;
    if(last_open_dir.length() != 0)
        chooser.set_dir(last_open_dir);
    filename = chooser.get_path();
    if(filename.length() != 0)
    {
#if (PLATFORM & WIN32) || (PLATFORM & WIN64)
        pos = filename.lastIndexOf("\\");
#elif (PLATFORM & LINUX32) || (PLATFORM & LINUX64)
        pos = filename.lastIndexOf("/");
#endif
        emit open_file_signal(filename);

    }

}


FileChooser::FileChooser(QWidget *parent, QString *dir)
    : QFileDialog(parent)
{
    this->setWindowTitle(tr(OPEN_FILE));
    if(dir)
    {
        this->setDirectory(*dir);
    }
    //this->setFilter(tr("(*)"));

}

QString&
FileChooser::get_path()
{
    if(exec() == QDialog::Accepted)
    {
        path = this->selectedFiles()[0];
    }
    else
        path = QString();
    return path;
}

void
FileChooser::set_dir(const QString &dir)
{
    this->setDirectory(dir);
}









