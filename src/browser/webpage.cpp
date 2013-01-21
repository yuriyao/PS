#include "webpage.h"

#include <QWebElement>
#include <QWebFrame>

WebPage::WebPage(QObject *parent) :
    QWebPage(parent)
{
    init();

}

WebPage::~WebPage()
{
    parser->~PythonParser();
    Ps_Delete(parser);
}

void
WebPage::init()
{
    parser = Ps_New<PythonParser>();
    new(parser) PythonParser(this);

    /*signal---slot*/
    connect(this->mainFrame(), SIGNAL(urlChanged(QUrl)), this, SLOT(page_url_changed_slot(QUrl)));

    /**/
    this->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

/*Struggle to get the main python path*/
QString
WebPage::get_python_url()
{
    QUrl ret;
    /**/
    QWebElement element = mainFrame()->findFirstElement("script[type='Ps'][__name__='__main__']");
    if(element.isNull())
    {
        element = mainFrame()->findFirstElement("script[type='PythonScript'][__name__='__main__']");
        if(element.isNull())
        {
             element = mainFrame()->findFirstElement("script[type='Ps']");
             if(element.isNull())
             {
                 element = mainFrame()->findFirstElement("script[type='PythonScript']");
                 if(element.isNull())
                     return QString();
             }
        }
    }
    /**/
    QString relative = element.attribute("src");
    if(!relative.isNull())
    {
        ret = page_url.resolved(QUrl(relative));
        return ret.toString();
    }
    return QString();
}

void
WebPage::page_url_changed_slot(QUrl url)
{
    this->page_url = url;
}







