#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPixmap>
#include <QIcon>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>

#include "MM.h"
#include "arraylist.h"

#define COLLECTED_PATH "../icons/collected.png"
#define UNCOLLECTED_PATH "../icons/uncollected.png"
#define BACKWARD_PATH "../icons/backward32.png"
#define FORWARD_PATH "../icons/forward32.png"
#define PAGE_PATH "../icons/page.png"
#define REFRESH_PATH "../icons/refresh.png"
#define LABEL_PATH "../icons/label.png"

#define TOOLBAR_DEBUG

class IconButton : public QLabel
{
    Q_OBJECT
public:
    IconButton(QWidget *parent = 0);
    IconButton(QPixmap &map, QWidget *parent = 0);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);

signals:
    void clicked();

public slots:

private:
    bool single_click;
};

class SearchBox : public QWidget
{
    Q_OBJECT
public:
    SearchBox(QWidget *parent = 0);
    virtual ~SearchBox();

public slots:
    void set_icon(QPixmap &icon);
    void set_collected(bool b);
    void set_url(QString &url);

public:
    /**/
    enum OpenType {WEB, DOWNLOAD, SRC, INTERNAL};

signals:
    void open_signal(QString &url, enum OpenType type);
    void collect_signal();

private:
    void init();
    void patch_signal();

private slots:
    void collect_slot();
    void open_slot();
    void add_widget();

private:
    QLabel *icon_label;
    QLineEdit *search_line;
    //QLabel *star_label;
    IconButton *star_button;
    QHBoxLayout *main_layout;
    QHBoxLayout *star_layout;
    const QPixmap page_map;
    const QPixmap collected_icon;
    const QPixmap uncollected_icon;
    bool collected;
};

class LabelItem
{
public:
    LabelItem();
    LabelItem(const QString &name, const QString &url);
    LabelItem(const QString &name, const QString &url, const QIcon &icon);

public:
    QListWidgetItem title_item;
    QString url;
};

class LabelsList : public QListWidget
{
    Q_OBJECT
public:
    LabelsList(QWidget *parent = 0);
    void show_pos(int x, int y);

signals:
    void row_clicked_signal(int row);

protected:
    virtual void focusOutEvent(QFocusEvent *event);

private slots:
    void current_row__changed_slot(int currentRow);
    void item_clicked_slot(QListWidgetItem *item);

private:
    int current_row;

};

class Labels : public QPushButton
{
    Q_OBJECT
public:
    Labels(QWidget *parent = 0);
    virtual ~Labels();

public slots:
    void add_item(const QString &name, const QString &url);
    void add_item(const QString &name, const QString &url, const QIcon &icon);
    void add_item(LabelItem &item);
    void show_list();

signals:
    void open_signal(QString &url, SearchBox::OpenType type);

private slots:
    void item_clicked_slot(int index);

private:


private:
    ArrayList<LabelItem> items;
    LabelsList *labels_list;
};





class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(QWidget *parent = 0);
    
signals:
    void open_signal(QString &url, enum SearchBox::OpenType type);
    void collect_signal();
    void back_signal();
    void forward_signal();
    void refresh_signal();

public slots:
    void set_icon_slot(QPixmap &icon);
    void set_collected_slot(bool b);
    void set_back_enable_slot(bool b);
    void set_forward_enable_slot(bool b);
    void add_item_slot(QString &name, QString &url);
    void add_item_slot(QString &name, QString &url, QIcon &icon);
    void set_url(QString &url);

private:
    void init();
    void patch_signal();
private:
    QPushButton *back_button;
    QPushButton *forward_button;
    QPushButton *refresh_button;
    SearchBox *search_box;
    Labels *label_button;
    QWidget *parent;
    //QHBoxLayout *main_layout;
};

#endif // TOOLBAR_H
