#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QUrl>
#include <QWebView>

#include "MM.h"
#include "pythonparser.h"

class WebPage : public QWebPage
{
    Q_OBJECT
public:
    enum LINK_OPEN_METHOD{};

    WebPage(QObject *parent = 0);

    virtual ~WebPage();

    QString get_python_url();


    
signals:
    
public slots:

private slots:
    void page_url_changed_slot(QUrl url);

private:
    void init();

private:
    PythonParser *parser;
    QUrl page_url;
};

#endif // WEBPAGE_H
