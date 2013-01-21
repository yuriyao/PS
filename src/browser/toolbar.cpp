#include "toolbar.h"
#include <iostream>

/********************LabelItem************************/
LabelItem::LabelItem()
{
}

LabelItem::LabelItem(const QString &name, const QString &url)
{
    title_item.setText(name);
    this->url = url;
}

LabelItem::LabelItem(const QString &name, const QString &url, const QIcon &icon)
{
    title_item.setText(name);
    title_item.setIcon(icon);
    this->url = url;
}

/*********************LabelsList**********************/
LabelsList::LabelsList(QWidget *parent) :
    QListWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:black");
    this->setStyleSheet("color:white");
    this->setFocusPolicy(Qt::ClickFocus);
    connect(this, SIGNAL(currentRowChanged(int)), this, SLOT(current_row__changed_slot(int)));
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(item_clicked_slot(QListWidgetItem*)));

}

void
LabelsList::show_pos(int x, int y)
{
    this->move(x, y);
    this->setCurrentRow(0);
    this->show();
}

void
LabelsList::current_row__changed_slot(int currentRow)
{
    current_row = currentRow;
}

void
LabelsList::item_clicked_slot(QListWidgetItem *item)
{
    emit row_clicked_signal(current_row);
    this->setVisible(false);
}

void
LabelsList::focusOutEvent(QFocusEvent *event)
{
    this->setVisible(false);
}

/********************Labels***************************/
Labels::Labels(QWidget *parent) :
    QPushButton(parent)
{
    this->setIcon(QIcon(QPixmap(LABEL_PATH)));
    labels_list = Ps_New<LabelsList>();
    new(labels_list) LabelsList(parent);
    connect(this, SIGNAL(clicked()), this, SLOT(show_list()));
    connect(labels_list, SIGNAL(row_clicked_signal(int)), this, SLOT(item_clicked_slot(int)));

}

Labels::~Labels()
{

}

void
Labels::item_clicked_slot(int index)
{
    LabelItem item;
    if(items.at(index, &item))
    {
        emit open_signal(item.url, SearchBox::WEB);
    }
}

void
Labels::add_item(const QString &name, const QString &url)
{
    LabelItem item(name, url);
    add_item(item);
}

void
Labels::add_item(const QString &name, const QString &url, const QIcon &icon)
{
    LabelItem item(name, url, icon);
    add_item(item);
}

void
Labels::add_item(LabelItem &item)
{
    items.append(item);
    labels_list->addItem(&item.title_item);
}

void
Labels::show_list()
{
    QRect rect = this->geometry();
    int x = rect.x() + rect.width() / 2;
    int y = rect.y() + rect.height();
    std::cout << x << " " << rect.x() << " " << rect.width() << std::endl;
    std::cout << y << " " << rect.y() << " " << rect.height() << std::endl;
    labels_list->show_pos(x, y);

}

/********************IconButton***********************/
IconButton::IconButton(QWidget *parent)
    : QLabel(parent),
      single_click(false)
{
}

IconButton::IconButton(QPixmap &map, QWidget *parent) :
    QLabel(parent)
{
    this->setPixmap(map);
}

void
IconButton::mouseReleaseEvent(QMouseEvent *ev)
{
    ev = ev;
    if(single_click)
    {
        single_click = false;
        emit clicked();
    }
}

void
IconButton::mousePressEvent(QMouseEvent *ev)
{
    ev = ev;
    single_click = true;
}

/*****************SearchBox*************************/
SearchBox::SearchBox(QWidget *parent) :
    QWidget(parent),
    collected(false),
    collected_icon(COLLECTED_PATH),
    uncollected_icon(UNCOLLECTED_PATH),
    page_map(PAGE_PATH)
{
    init();
    patch_signal();
    add_widget();
}

SearchBox::~SearchBox()
{
    icon_label->~QLabel();
    Ps_Delete(icon_label);
    search_line->~QLineEdit();
    Ps_Delete(search_line);
    //star_label->~QLabel();
    //Ps_Delete(star_label);
    star_button->~IconButton();
    Ps_Delete(star_button);
    main_layout->~QHBoxLayout();
    Ps_Delete(main_layout);
    star_layout->~QHBoxLayout();
    Ps_Delete(star_layout);
}

void
SearchBox::init()
{
    /*Init icon label*/
    icon_label = Ps_New<QLabel>();
    new(icon_label) QLabel();
    icon_label->setPixmap(QPixmap(PAGE_PATH));
    icon_label->setMaximumHeight(32);
    /*Init search Line*/
    search_line = Ps_New<QLineEdit>();
    new(search_line) QLineEdit();
    /*Init star button*/
    star_button = Ps_New<IconButton>();
    new(star_button) IconButton();
    star_button->setPixmap(QPixmap(UNCOLLECTED_PATH));
    //star_button->setBackgroundRole(QPalette::NoRole);
    //star_button->setFlat(true);
    /*Init star layout*/
    star_layout = Ps_New<QHBoxLayout>();
    new(star_layout) QHBoxLayout();
    star_layout->addWidget(star_button);
    /*Init star label*/
    //star_label = Ps_New<QLabel>();
   // new(star_label) QLabel();
    //star_label->setLayout(star_layout);
    /*Init main layout*/
    main_layout = Ps_New<QHBoxLayout>();
    new(main_layout) QHBoxLayout();
}

void
SearchBox::add_widget()
{
    main_layout->addWidget(icon_label);
    main_layout->addWidget(search_line);
    main_layout->addLayout(star_layout);
    this->setLayout(main_layout);
}

void
SearchBox::set_icon(QPixmap &icon)
{
    icon_label->setPixmap(icon);
}

void
SearchBox::set_collected(bool b)
{
    collected = b;
    if(b)
        star_button->setPixmap(collected_icon);
    else
        star_button->setPixmap(uncollected_icon);
}

void
SearchBox::set_url(QString &url)
{
    search_line->setText(url);
}

void
SearchBox::collect_slot()
{
    if(!collected)
    {
        collected = true;
        set_collected(true);
        star_button->setPixmap(collected_icon);
        emit collect_signal();
    }
}

void
SearchBox::open_slot()
{
    QString url = search_line->text();
    if(url.length() > 0)
    {
        emit open_signal(url, WEB);
    }
}

void
SearchBox::patch_signal()
{
    connect(star_button, SIGNAL(clicked()), this, SLOT(collect_slot()));
    connect(search_line, SIGNAL(editingFinished()), this, SLOT(open_slot()));
}

ToolBar::ToolBar(QWidget *parent) :
    QToolBar(parent), parent(parent)
{
    init();
}

/*********************ToolBar***************************/
void
ToolBar::init()
{
    /*Init back_button*/
    back_button = Ps_New<QPushButton>();
    new(back_button) QPushButton();
    back_button->setIcon(QIcon(QPixmap(BACKWARD_PATH)));
    back_button->setFlat(true);
    /*Init forward button*/
    forward_button = Ps_New<QPushButton>();
    new(forward_button) QPushButton();
    forward_button->setIcon(QIcon(QPixmap(FORWARD_PATH)));
    forward_button->setFlat(true);
    /*Init refresh button*/
    refresh_button = Ps_New<QPushButton>();
    new(refresh_button) QPushButton();
    refresh_button->setIcon(QIcon(QPixmap(REFRESH_PATH)));
    refresh_button->setFlat(true);
    /*Init search box*/
    search_box = Ps_New<SearchBox>();
    new(search_box) SearchBox();
    //search_box->setStyleSheet("border: 2px solid black");
    //search_box->setStyleSheet("border-style:outset");
    /*Init main layout*/
    //main_layout = Ps_New<QHBoxLayout>();
    //new(main_layout) QHBoxLayout();
    /*Init label button*/
    label_button = Ps_New<Labels>();
    new(label_button) Labels(0);
    label_button->setIcon(QIcon(QPixmap(LABEL_PATH)));
    label_button->setFlat(true);

    this->addWidget(back_button);
    this->addWidget(forward_button);
    this->addWidget(refresh_button);
    this->addSeparator();
    this->addWidget(search_box);
    this->addSeparator();
    this->addWidget(label_button);

    this->setMovable(false);
    //this->setLayout(main_layout);

}

void
ToolBar::set_icon_slot(QPixmap &icon)
{
    search_box->set_icon(icon);
}

void
ToolBar::set_collected_slot(bool b)
{
    search_box->set_collected(b);
}

void
ToolBar::set_back_enable_slot(bool b)
{
    back_button->setEnabled(b);
}

void
ToolBar::set_forward_enable_slot(bool b)
{
    forward_button->setEnabled(b);
}

void
ToolBar::set_url(QString &url)
{
    search_box->set_url(url);
}

void
ToolBar::add_item_slot(QString &name, QString &url)
{
    label_button->add_item(name, url);
}

void
ToolBar::add_item_slot(QString &name, QString &url, QIcon &icon)
{
    label_button->add_item(name, url, icon);
}

void
ToolBar::patch_signal()
{
    connect(search_box, SIGNAL(open_signal(QString&,OpenType)), this, SIGNAL(open_signal(QString&,SearchBox::OpenType)));
    connect(search_box, SIGNAL(collect_signal()), this, SIGNAL(collect_signal()));
    connect(back_button, SIGNAL(clicked()), this, SIGNAL(back_signal()));
    connect(forward_button, SIGNAL(clicked()), this, SIGNAL(forward_signal()));
    connect(refresh_button, SIGNAL(clicked()), this, SIGNAL(refresh_signal()));
}

















