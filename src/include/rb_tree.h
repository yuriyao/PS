/**
 *  实现红黑树
 **/
#ifdef _cplusplus
extern "C"
{
#endif
#include "size.h"
#include "mm.h" 
#include "log.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define RED 0
#define BLACK 1

//#define RB_DEBUG

struct rb_tree
{
	void *value;
	int color;
	struct rb_tree *parent;
	struct rb_tree *lchild;
	struct rb_tree *rchild; 
};

struct _RB_tree 
{
	struct rb_tree **root;
	int (*compare_func)(void *a, void *b);
	void (*clear_func)(void *value);
	void* (*malloc_func)(void*);
#ifdef RB_DEBUG
	//跟踪rb-tree的节点数量
	int nodes_num;
#endif
};

typedef struct _RB_tree* RB_tree;


/**
 *  创建一个rb树
 *  value 节点的值，是根节点的值
 *  compare 函数指针，如果a>b，返回正值;等于返回0;小于返回负值
 *  如果compare为空，则直接比较value的指针大小
 *  clear_func 用于释放动态内存分配的value资源的函数
 **/
RB_tree Ps_CreateRB(void *value, int (*compare_func)(void *a, void *b), void* (*malloc_func)(void*), void (*clear_func)(void *value));

/**
 *
 **/
BOOL Ps_RBInsert(RB_tree tree, void *value);

/**
 *  
 **/

BOOL Ps_RBDel(RB_tree tree, void *value);

void* Ps_RBFind(RB_tree tree, void *value);


/** 
 *  释放rb-tree的内存
 *  tree
 **/
void Ps_RBFree(RB_tree tree);

#ifdef _cplusplus
}
#endif

