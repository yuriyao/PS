#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QWebView>
#include <python2.6/Python.h>
#include <python2.6/pythread.h>

#include "webpage.h"

struct Web_Info
{
    WebPage *page;
    QWebView *view;
};

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = 0);

signals:

public slots:

private:

};

#endif // TAB_H
