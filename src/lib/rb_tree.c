#include "../include/rb_tree.h"

/*哨兵节点*/
struct rb_tree _NIL = 
{
	NULL,
	BLACK,
	NULL,
	NULL,
	NULL,
};

/*共用的哨兵节点*/
struct rb_tree *NIL = &_NIL;

static void  _rb_free(RB_tree tree, struct rb_tree *root);

/** 
 *  默认的比较两个a和b的函数
 **/
static int 
compare_default(void *a, void *b)
{
	return (a > b) ? 1 : ((a < b) ? -1 : 0);
}

/**
 *  默认的内存分配函数
 **/
static void*
malloc_defalut(void *value)
{
	return value;
}

/**
 *  默认的内存释放函数
 **/
static void
clear_defalut(void *value)
{
	return;
}

RB_tree 
Ps_CreateRB(void *value, 
	int (*compare_func)(void *a, void *b), 
	void* (*malloc_func)(void*), 
	void (*clear_func)(void *value))
{
	struct _RB_tree *ret = (struct _RB_tree*)Ps_Malloc(sizeof(struct _RB_tree));
	struct rb_tree **root = (struct rb_tree**)Ps_Malloc(sizeof(struct rb_tree*));
	if(!ret || !root)
	{
		Ps_Log("Malloc memery for RB_tree failed\n", Ps_LOG_WARING);
		return NULL;
	}
	*root = (struct rb_tree*)Ps_Malloc(sizeof(struct rb_tree));
	if(!root)
	{
		Ps_Log("Malloc memery for RB_tree's node failed\n", Ps_LOG_WARING);
		Ps_Free(ret);
		return NULL;
	}
	/*初始化rb-tree*/
	ret->root = root;
	ret->compare_func = (compare_func) ? (compare_func) : compare_default;
	ret->malloc_func = (malloc_func) ? malloc_func : malloc_defalut;
	ret->clear_func = clear_func ? clear_func : clear_defalut;
#ifdef RB_DEBUG
	ret->nodes_num = 1;
#endif
	/*初始化rb-tree节点*/
	(*root)->value = (ret->malloc_func)(value);
	(*root)->color = BLACK;
	(*root)->parent = NULL;
	(*root)->lchild = NIL;
	(*root)->rchild = NIL;
	return ret;
}

BOOL 
Ps_RBInsert(RB_tree tree, void *value)
{
	void *_value;
	struct rb_tree *node;
	struct rb_tree *root;
	int (*compare_func)(void *a, void *b);
	int t;

	if(tree == NULL)
	{
		Ps_Log("Want to insert into NULL RB_tree\n", Ps_LOG_WARING);
		return FALSE;
	}
	assert(tree->malloc_func);
	assert(tree->compare_func);
	assert(tree->root);
	assert(*(tree->root));

#ifdef RB_DEBUG
	tree->nodes_num ++;
#endif

	_value = tree->malloc_func(value);
	/*申请并初始化待插入的节点*/
	node = (struct rb_tree*)Ps_Malloc(sizeof(struct rb_tree));
	if(node == NULL)
	{
		Ps_Log("Malloc memery for struct rb_tree failed\n", Ps_LOG_WARING);
		return FALSE;
	}
	node->value = _value;
	node->color = RED;
	node->lchild = NIL;
	node->rchild = NIL;

	compare_func = tree->compare_func;
	root = *(tree->root);

	/*使用普通的二叉树*/
	while(1)
	{
		t = compare_func(value, root->value);
		if(t > 0)
		{
			if(root->rchild == NIL)
			{
				root->rchild = node;
				node->parent = root;
				return TRUE;
			}
			else
				root = root->rchild;
		}
		else if(t <= 0)
		{
			if(root->lchild == NIL)
			{
				root->lchild = node;
				node->parent = root;
				return TRUE;
			}
			else
				root = root->lchild;
		}
	}
	/*OVER*/
}

BOOL Ps_RBDel(RB_tree tree, void *value)
{
	return TRUE;
}

void* Ps_RBFind(RB_tree tree, void *value)
{
	struct rb_tree *root;
	int (*compare_func)(void *a, void *b);
	int tmp;
	if(!tree)
	{
		Ps_Log("Want to find NULL RB_tree\n", Ps_LOG_WARING);
		return (void**)0;
	}
	assert(tree->compare_func);
	assert(tree->root);
	assert(*(tree->root));
	compare_func = tree->compare_func;
	root = *(tree->root);
	while(root != NIL)
	{
		tmp = compare_func(value, root->value);
		if(tmp > 0)
			root = root->rchild;
		else if(tmp < 0)
			root = root->lchild;
		else
			return (root->value);
	}
	return (void*)0;
}


void Ps_RBFree(RB_tree tree)
{
	struct rb_tree *root;
	if(tree == NULL)
	{
		Ps_Log("Want to free NULL RB_tree\n", Ps_LOG_WARING);
		return;
	}
	root = *tree->root;
	assert(root);
	_rb_free(tree, root);
}

static void 
_rb_free(RB_tree tree, struct rb_tree *root)
{
	assert(tree);
	assert(tree->clear_func);
	if(root != NIL)
	{
		_rb_free(tree, root->lchild);
		_rb_free(tree, root->rchild);
		tree->clear_func(root->value);
		Ps_Free(root);
#ifdef RB_DEBUG
		tree->nodes_num --;
#endif
	}
}

#ifdef RB_DEBUG
#include "../include/init_clear.h"
static void
traverse(struct rb_tree *root)
{
	if(root != NIL)
	{
		printf("%d\n", (int)root->value);
		traverse(root->lchild);
		traverse(root->rchild);
	}
}

static void 
show_rb(RB_tree tree)
{
	printf("nodes number is %d\n", tree->nodes_num);
	traverse(*(tree->root));
}

int main(int argc, char **argv)
{
	int value[] = {3, 5, 4, 2, 1, 0, 7, 6, 3, 1, 4};
	int i = 0;
	void *ret;
	//Ps_MMInit();
	//Ps_InitLog(NULL, NULL, NULL);
	Ps_INIT(NULL, NULL, NULL);
	RB_tree tree = Ps_CreateRB((void*)2, NULL, NULL, NULL);
	for(i = 0; i < sizeof(value) / sizeof(int); i ++)
		Ps_RBInsert(tree, (void*)value[i]);
	show_rb(tree);
	ret = Ps_RBFind(0, (void*)3);
	//printf("%d\n", (int)(*ret));
	Ps_RBFree(tree);
	//Ps_CloseLog();
	//Ps_MMClear();
	Ps_CLEAR();
	return 0;
}

#endif
