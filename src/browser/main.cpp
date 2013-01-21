#include <QtWidgets/QApplication>
#include <stdio.h>
#include "../include/init_clear.h"
#include "browser.h"
#include "toolbar.h"

/*日志文件名*/
#define LOG_FILE "log.txt"

int 
main(int argc, char **argv)
{
    QApplication app(argc, argv);
    int ret;
    Browser browser;
    SearchBox box;
    //日志文件
    FILE *file;
    file = fopen(LOG_FILE, "w");
    //系统初始化
    if(!file)
    {
        printf("Open %s as log file failed\n", LOG_FILE);
        Ps_INIT(NULL, NULL, NULL);
    }
    else
        Ps_INIT(file, file, file);

    /*Run browser*/
    browser.show();
    //box.show();
    
    ret = app.exec();
    //系统清理
    Ps_Log((char*)"Clear system to exit\n", Ps_LOG_NORMAL);
    Ps_CLEAR();
    return ret;
}
