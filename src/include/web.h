/**
 * 处理web相关的函数
 **/
#ifndef _WEB_H
#define _WEB_H

#ifdef _cplusplus
extern "C"
{
#endif

#include <curl/curl.h>
#include <stdio.h>
#include "config.h"
#include "log.h"
#include "size.h"

/**
 *  只能调用一次的初始化函数
 *  ssl 是否启用ssl
 **/
BOOL Ps_WebInit(BOOL ssl);

/**
 *  下载文件
 *  url 
 *  p 写入的文件
 **/
BOOL Ps_Download(char *url, FILE *p);

/**
 *  下载文件（只是多了超时的时间）
 *  timeout 超时时间（单位秒）
 **/
BOOL Ps_DownloadTimeout(char *url, FILE *p, int timeout);

/**
 * 只能调用一次的清理函数
 **/
BOOL Ps_WebClear();

#define WEB_DEBUG

#ifdef _cplusplus
}
#endif

#endif