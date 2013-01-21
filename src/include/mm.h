#include "../include/size.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/*启用的内存管理策略*/
	#define MM_POLICY 1
	/**
	 *  分配size大小的内存
	 **/
	void* Ps_Malloc(long size);

	/**
	 *  释放内存
	 **/
	void Ps_Free(void *p);

	/**
	*  内存管理初始化
	**/
	BOOL Ps_MMInit();

	/**
	 *  内存管理的清理
	 **/
	 BOOL Ps_MMClear();

	//#define MM_DEBUG
#ifdef __cplusplus
}
#endif
