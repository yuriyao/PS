#include <stdlib.h>
#include "../include/log.h"
#include <python2.6/pythread.h>
#include <assert.h>
#include "../include/mm.h"

/*锁*/
static PyThread_type_lock *mm_lock;
#define MM_INIT_LOCK() PyThread_allocate_lock()
#define MM_GET_LOCK() PyThread_acquire_lock(mm_lock, WAIT_LOCK)
#define MM_RELEASE_LOCK() PyThread_release_lock(mm_lock)
#define MM_FREE_LOCK() PyThread_free_lock(mm_lock)

/**/
#define BLOCK_BASE 8
/**/
#define BLOCK_SHIFT 3
/*使用内存管理的内存大小的界限*/
#define SMALL_MEM_LIMIT 256
/**/
#define SIZE2INDEX(size) ((size - 1) >> BLOCK_SHIFT)
#define INDEX2SIZE(index) ((index + 1) << BLOCK_SHIFT)

/*字节对齐*/
#define ALIGN BLOCK_BASE
#define ALIGN_MASK (ALIGN - 1)
#define ROUND(size) ((size + ALIGN_MASK) & ~ALIGN_MASK)

/**
 *  内存管理方案一
 *  方案说明：
 *  每块申请的内存由两部分组成:
 *  指示内存大小的head部分和实际使用的内存
 *  每一个mm_struct维护一个固定长度的内存，
 *  每次分配内存时现在一定宽度的范围内寻找适
 *  合的块，并对剩余的块进行相应的处理,
 *  如果没有找到再进行内存的分配
 *   mm_structs--------------------------
 *  		   |frees|frees|frees|....
 *             ------------------\-------
 *             |block|block|block|\...
 *               \                 \
 *                \                 \
 *                 \               [ 长度为8 * i的内存,内容为下一个内存片段的长度]->[]->[0]
 *                  mm_block->mm_block->null
 */
#if MM_POLICY == 0

/*其大小必须已经对齐*/
struct mm_head
{
	long size;
};

/*管理一大块内存块,一般为4K*/
struct mm_block
{
	//空闲块的数量
	long free_number;
	//空闲块的开始地址
	void *frees;
	//
	void *begin;
	//struct mm_block *prev;
	struct mm_block *next;
};

struct mm_struct
{
	/*指向内存特定长度的片段的链表头*/
	void *frees;
	/**/
	struct mm_block *block;
};

struct mm_big_blocks
{
	struct mm_big_blocks *next;
	void *p;
};

/*不同内存片段的数量*/
#define BLOCK_NUMBER (SMALL_MEM_LIMIT >> BLOCK_SHIFT)

//获得mm_struct所维护的一次分配的内存的大小
#define GET_SIZE(mm_struct) ((mm_struct - mm_structs) / sizeof(struct mm_struct) << BLOCK_SHIFT) 
/*每个内存块的大小*/
#define BLOCK_SIZE (4 * 1024)
/*一次申请的block的块数*/
#define BLOCK_ONE_TIME 32
/*每次寻找的相邻块的宽度*/
#define BLOCK_WIDTH 2
#define WRAP_TO_RET(ret, size)\
do{\
	((struct mm_head*)ret)->size = size;\
	ret += sizeof(struct mm_head);\
	MM_RELEASE_LOCK();\
	return ret;\
}while(0)


#define BLOCK_STRUCT_SIZE (ROUND(sizeof(struct mm_block)))
#define GET_POINTER(head) ((void*)((struct mm_head*) + 1))
#define GET_HEAD(p) ((struct mm_head*)p - 1)

static struct mm_block* get_free_block();
static int get_perfect_index(long size);

static struct mm_struct mm_structs[BLOCK_NUMBER];

static struct mm_block *free_blocks;
//维护每次申请的大块内存
static struct mm_big_blocks *blocks_head;


BOOL Ps_MMInit()
{
	int i = 0;
	mm_lock = MM_INIT_LOCK();
	if(!mm_lock)
		return FALSE;
	MM_GET_LOCK();
	for(i = 0; i < BLOCK_NUMBER; i ++)
	{
		mm_structs[i].frees = (void*)0;
		mm_structs[i].block = (struct mm_block*)0;
	}
	free_blocks = (struct mm_block*)0;
	blocks_head = (struct mm_big_blocks*)0;
	MM_RELEASE_LOCK();
	return TRUE;
}

void* Ps_Malloc(long size)
{
	struct mm_block *block;
	char *ret;
	int index;
	int d = size;

	size += sizeof(struct mm_head);
	index = SIZE2INDEX(size);
	size = ROUND(size);

	//待申请的内存太大，使用普通的内存分配方法
	if(size > SMALL_MEM_LIMIT)
	{
		ret = (char*)malloc(size);
		/*((struct mm_head*)ret)->size = size;
		ret += sizeof(struct mm_head);
		return ret;*/
		WRAP_TO_RET(ret, size);
	}
	MM_GET_LOCK();
	if(mm_structs[index].frees)
	{
		ret = (char*)(mm_structs[index].frees);
		mm_structs[index].frees = (void*)(*(long*)ret);
		/*((struct mm_head*)ret)->size = size;
		ret += sizeof(struct mm_head);
		return ret;*/
		WRAP_TO_RET(ret, size);
	}

	//没有找到适合的内存块
	if(get_perfect_index(size) == -1)
	{
		block = get_free_block();
		if(!block)
		{
			MM_RELEASE_LOCK();
			return NULL;
		}
		block->next = mm_structs[index].block;
		mm_structs[index].block = block;
		block->begin = (void*)((char*)block + BLOCK_STRUCT_SIZE);
		ret = block->begin;
		block->free_number = (BLOCK_SIZE - BLOCK_STRUCT_SIZE) / size - 1;
		block->frees = (void*)((char*)ret + size);
		WRAP_TO_RET(ret, size);
	}
	else
	{
		int pi = get_perfect_index(size);
		int tmp;
		int remain;
		assert(mm_structs[pi].block->free_number > 0);
		if(pi < index)
		{
			tmp = (index + 1) / (pi + 1);
			tmp = (tmp * (pi + 1) == index + 1) ? tmp : (tmp + 1);
			ret = mm_structs[pi].block->frees;
			remain = tmp * INDEX2SIZE(pi) - size;
			assert(tmp <= mm_structs[pi].block->free_number);
			mm_structs[pi].block->free_number -= tmp;
			mm_structs[pi].block->frees = (void*)((char*)mm_structs[pi].block->frees + tmp * INDEX2SIZE(pi));

			assert(remain >= 0);
			assert(remain < size);
			if(remain)
			{
				struct mm_head *head = (struct mm_head*)(ret + size);
				*(long*)head = (long)mm_structs[SIZE2INDEX(remain)].frees;
				mm_structs[SIZE2INDEX(remain)].frees = (void*)head;
			}
			WRAP_TO_RET(ret, size);
		}
		else if(pi == index)
		{
			mm_structs[index].block->free_number --;
			ret = (char*)mm_structs[index].block->frees;
			mm_structs[index].block->frees = (void*)((char*)ret + size);
			WRAP_TO_RET(ret, size);
		}
		else
		{
			void *rp;
			mm_structs[pi].block->free_number --;
			ret = (char*)mm_structs[pi].block->frees;
			mm_structs[pi].block->frees = (void*)((char*)ret + INDEX2SIZE(pi));
			remain = INDEX2SIZE(pi) - size;
			rp = ret + size;
			*(long*)rp = (long)mm_structs[SIZE2INDEX(remain)].frees;
			mm_structs[SIZE2INDEX(remain)].frees = rp;
			WRAP_TO_RET(ret, size);
		}

	}

}

void 
Ps_Free(void *p)
{
	struct mm_head *head = GET_HEAD(p);
	int size = head->size;
	assert(size > 0);
	
	if(size > SMALL_MEM_LIMIT)
	{
		free((void*)head);
	}
	else
	{
		assert(size / BLOCK_BASE * BLOCK_BASE == size);
		MM_GET_LOCK();
		*(long*)head = (long)mm_structs[SIZE2INDEX(size)].frees;
		mm_structs[SIZE2INDEX(size)].frees = head;
		MM_RELEASE_LOCK();
	}
}

BOOL Ps_MMClear()
{
	struct mm_big_blocks *bb;
#ifdef MM_DEBUG
	int i = 0;
#endif
	//释放所有申请的内存
	MM_GET_LOCK();
	while(blocks_head)
	{
#ifdef MM_DEBUG
		i ++; 
#endif
		bb = blocks_head;
		blocks_head = blocks_head->next;
		free(bb->p);
		free(bb);
	}
#ifdef MM_DEBUG
	printf("MALLOC big blocks is %d\n", i);
#endif
	//printf("MALLOC big blocks is %d\n", 10);
	memset(mm_structs, 0, sizeof(mm_structs));
	MM_RELEASE_LOCK();
	//释放锁
	MM_FREE_LOCK();
	return TRUE;
}

static struct mm_block*
get_free_block()
{
	int i = 0;
	struct mm_block *tmp;
	struct mm_big_blocks *bb;
#ifdef MM_DEBUG
	static int max_number = 0;
#endif
	if(!free_blocks)
	{
#ifdef MM_DEBUG
		max_number ++;
		printf("Malloc max_number is %d\n", max_number);
#endif
		//int len = BLOCK_ONE_TIME * BLOCK_SIZE;
		free_blocks = (struct mm_block*)malloc(BLOCK_ONE_TIME * BLOCK_SIZE);
		bb = (struct mm_big_blocks*)malloc(sizeof(struct mm_big_blocks));
		if(bb)
		{
			bb->next = blocks_head;
			bb->p = free_blocks;
			blocks_head = bb;
		}
		if(!free_blocks)
			return NULL;
		for(i = 0; i < BLOCK_ONE_TIME; i ++)
		{
			tmp = (struct mm_block*)((char*)free_blocks + i * BLOCK_SIZE);
			tmp->next = (struct mm_block*)((char*)tmp + BLOCK_SIZE);
		}
		tmp->next = (struct mm_block*)0;
	}
	tmp = free_blocks;
	free_blocks = tmp->next;
	return tmp;
}

/**
 *  获得最适合的大小的块的索引
 **/
static int 
get_perfect_index(long size)
{
	int index = SIZE2INDEX(size);
	int i;
	int low = index - BLOCK_WIDTH;
	int high = index + BLOCK_WIDTH;
	low = (low < 0) ? 0 : low;
	high = (high >= BLOCK_NUMBER) ? BLOCK_NUMBER : high;
	//先寻找自身和比自身大的块
	for(i = index; i < high; i ++)
	{
		if(mm_structs[i].block && mm_structs[i].block->free_number > 0)
			return i;
	}
	for(i = index - 1; i >= low; i --)
	{
		int tmp = (index + 1) / (i + 1);
		tmp = (tmp * (i + 1) == index + 1) ? tmp : (tmp + 1);
		if(mm_structs[i].block && mm_structs[i].block->free_number >= tmp)
			return i;
	}
	return -1;
}
#endif

/**
 *  使用内存分配方案二
 **/
#if MM_POLICY == 1
/**
 *  分配size大小的内存
 **/
void* Ps_Malloc(long size)
{
	return malloc(size);
}

/**
 *  释放内存
 **/
void Ps_Free(void *p)
{
	free(p);
}

/**
*  内存管理初始化
**/
BOOL Ps_MMInit()
{

}

/**
 *  内存管理的清理
 **/
BOOL Ps_MMClear()
{

}

#endif



/*进行调试*/
#ifdef MM_DEBUG
#define MM_TEST_NUM 256
#define MM_TEST_HIGH 256

int main(int argc, char **argv)
{
	int i = 0;
	int k = 0;
	void *p[MM_TEST_NUM][MM_TEST_HIGH];
	void *pi;
	Ps_MMInit();
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		for(k = 0; k < MM_TEST_HIGH; k ++)
		{
			p[i][k] = Ps_Malloc(i);
			*(char*)p[i][0] = (char)i;
		}
	}
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		printf("%d ", *(char*)p[i][0]);
	}
	printf("\n");
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		for(k = 0; k < MM_TEST_HIGH; k ++)
			Ps_Free(p[i][k]);
	}
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		p[i][0] = Ps_Malloc(i);
		*(char*)p[i][0] = (char)i;
	}
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		printf("%d ", *(char*)p[i][0]);
	}
	printf("\n");
	for(i = 0; i < MM_TEST_NUM; i ++)
	{
		
		Ps_Free(p[i][0]);
	}

	Ps_MMClear();
	return 0;
}
#endif