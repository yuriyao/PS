#ifndef PYTHONPARSER_H
#define PYTHONPARSER_H

#include <QThread>
#include <QWebPage>

class PythonParser : public QThread
{
    Q_OBJECT
public:
    explicit PythonParser(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PYTHONPARSER_H
