/**
*  Ps 的log
**/
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <python2.6/Python.h>
#include <python2.6/pythread.h> 
#include <stdio.h>
#include "../include/size.h"

#define LOG_LOCK_ENABLE 1

#if LOG_LOCK_ENABLE
#define GET_LOG_LOCK() PyThread_acquire_lock(log_lock, WAIT_LOCK)
#define RELEASE_LOG_LOCK() PyThread_release_lock(log_lock)
#else
#define GET_LOG_LOCK()
#define RELEASE_LOG_LOCK()
#endif

/**
 *  正常信息
 **/
#define Ps_LOG_NORMAL 0
/**
 *  警告信息
 **/
#define Ps_LOG_WARING 1

/**
 *  错误信息
 **/
#define Ps_LOG_ERROR 2

/**
 * 初始化Log系统
 * normal 普通信息输出的文件指针，默认为标准输出
 * error  错误信息输出的文件指针，默认为标准错误输出
 * waring 警告信息输出的文件指针，默认为标准输出
 * 成功返回TRUE,否则返回FALSE
 **/
BOOL Ps_InitLog(FILE *normal, FILE *error, FILE *waring);

/**
 *  打印log信息
 *  info log信息
 *  type log类型(可以为Ps_LOG_NORMAL, PS_LOG_WARING, PS_LOG_ERROR)
 **/
BOOL Ps_Log(char *info, int type);

/**
 *  打印有格式的log
 *  format 打印的log信息格式
 *  type   log类型(可以为Ps_LOG_NORMAL, PS_LOG_WARING, PS_LOG_ERROR)
 **/
BOOL Ps_LogFormat(char *format, int type, ...);

/**
 *  打印log到指定的文件
 *  info log信息
 *  type log类型(可以为Ps_LOG_NORMAL, PS_LOG_WARING, PS_LOG_ERROR)
 *  file 打印到的目标文件
 **/
BOOL Ps_LogFile(char *info, int type, FILE *file);

/**
 * 打印有格式的log到指定的文件
 * format log的格式
 * type   log类型(可以为Ps_LOG_NORMAL, PS_LOG_WARING, PS_LOG_ERROR)
 * file   log的目标文件
 **/
BOOL Ps_LogFileFormat(char *format, int type, FILE *file, ...);

/**
 * 打印python的对象的信息
 * object 待打印python对象
 * type   log类型(可以为Ps_LOG_NORMAL, PS_LOG_WARING, PS_LOG_ERROR) 
 **/
BOOL Ps_LogObject(PyObject *object, int type);

/**
 *  关闭log,log将不能继续使用，如果使用可能会发生段错误
 **/
BOOL Ps_CloseLog();

/*启动调试*/
//#define LOG_DEBUG 


#ifdef __cplusplus
}
#endif

#endif
