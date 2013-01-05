/**
 *  实现通用的map的数据结构
 **/
#ifndef _MAP_H
#define _MAP_H

#ifdef _cplusplus
extern "C"
{
#endif

#include "mm.h"
#include "log.h"
#include "rb_tree.h"
#include "size.h"

#define INT_LEN sizeof(int)

/*任何计算出来的hash值都应该与一下这个宏，使其肯定为正值*/
//#if INT_LEN = 4
#define Ps_MAP_MASK (1 << (8 * sizeof(int) - 1) - 1)
//#endif

/*#if 2 == sizeof(int)
#define Ps_MAP_MASK(1 << 15 - 1)
#endif

#if 8 == sizeof(int)
#define Ps_MAP_MASK(1 << 63 - 1)
#endif*/

/*还未计算hash值*/
#define Ps_NOT_HASH -1

#define MAP_DEBUG

struct Map;

struct map_node
{
	void *key;
	void *value;
	int hash;
	struct Map *map;
};

typedef struct Map
{
	RB_tree tree;
	int (*hash_func)(void *key);
	void* (*malloc_key)(void*);
	void* (*malloc_value)(void*);
	void (*free_key)(void*);
	void (*free_value)(void*);
	int (*compare_func)(void *k1, void *k2);
};

typedef struct Map* Map;

Map Ps_CreateMap(void *key, void *value, int (*hash_func)(void*),
	void* (*malloc_key)(void*), void* (*malloc_value)(void*), 
	void (*free_key)(void*), void (*free_value)(void*), 
	int (*compare_func)(void *k1, void *k2));

BOOL Ps_MapInsert(Map map, void *key, void *value);

void* Ps_MapFind(Map map, void *key);

BOOL Ps_MapDel(Map map, void *key);

/*专为key-value都是string设计的map*/
Map Ps_CreateMapStr(char *key, char *value);

BOOL Ps_MapInsertStr(Map map, char *key, char *value);

char* Ps_MapFindStr(Map map, char *key);

BOOL Ps_MapDelStr(Map map, char *key);


#ifdef _cplusplus
}
#endif

#endif