QT       += core gui
QT       += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ps
TEMPLATE = app

LIBS += -lpython2.6 -lcurl

SOURCES += \
    src/browser/main.cpp \
    src/browser/browser.cpp \
    src/browser/MM.cpp \
    src/browser/filemenu.cpp \
    src/browser/menubar.cpp \
    src/lib/mm.c \
    src/util/log.c \
    src/lib/web.c \
    src/browser/toolbar.cpp \
    src/browser/arraylist.cpp \
    src/browser/tab.cpp \
    src/browser/webpage.cpp \
    src/browser/pythonparser.cpp \
    src/browser/tabs.cpp \
    src/browser/centerlinear.cpp \
    src/lib/exception.cpp

HEADERS += \
    src/browser/browser.h \
    src/browser/MM.h \
    src/browser/filemenu.h \
    src/browser/menubar.h \
    src/include/config.h \
    src/include/init_clear.h \
    src/include/log.h \
    src/include/mm.h \
    src/include/web.h \
    src/browser/toolbar.h \
    src/browser/arraylist.h \
    src/browser/tab.h \
    src/browser/webpage.h \
    src/browser/pythonparser.h \
    src/browser/tabs.h \
    src/browser/centerlinear.h \
    src/include/exception.h



