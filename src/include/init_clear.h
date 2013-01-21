/**
 *  初始化和清理
 **/
#ifndef _INIT_CLEAR_H
#define _INIT_CLEAR_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "log.h"
#include "mm.h"
#include "web.h"

#define Ps_INIT(file_normal, file_error, file_waring)\
 Ps_MMInit();\
 Ps_InitLog(file_normal, file_error, file_waring);\
 Ps_WebInit(FALSE)

#define Ps_CLEAR()\
 Ps_WebClear();\
 Ps_CloseLog();\
 Ps_MMClear()


#ifdef __cplusplus
}
#endif

#endif
