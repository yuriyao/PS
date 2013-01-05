/**
 *  系统的配置文件
 **/
#ifndef _CONFIG_H
#define _CONFIG_H

#define LINUX32 1
#define LINUX64 2
#define WIN32   4
#define WIN64   8

/*PLATFORM 可以是LINUX32;LINUX64;WIN32;WIN64*/
#define PLATFORM LINUX32

/*网络上下载python文件的超时时间长度（单位：秒）*/
#define TIME_OUT 10

#endif
