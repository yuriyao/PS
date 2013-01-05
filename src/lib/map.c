#include "../include/map.h"
#include <assert.h>
#include <string.h>

static struct map_node*
create_Map_node(void *key, void *value, Map map)
{
	struct map_node *ret = (struct map_node*)Ps_Malloc(sizeof(struct map_node*));
	void *_key = key;
	void *_value = value;
	int hash = (int)(key);
	if(ret == NULL)
	{
		Ps_Log("Malloc memery for MapNode failed\n", Ps_LOG_WARING);
		return NULL;
	}
	/*map*/
	assert(map);
	if(map->malloc_key)
		_key = map->malloc_key(key);
	if(map->malloc_value)
		_value= map->malloc_value(value);
	if(map->hash_func)
		hash = map->hash_func(key);
	hash &= Ps_MAP_MASK;
	ret->key = _key;
	ret->value = _value;
	ret->hash = hash;
	ret->map = map;
	return ret;
} 

static void 
clear_map_node(void *node)
{
	struct map_node *_node = (struct map_node*)node;
	if(node == NULL)
	{
		Ps_Log("Free NULL struct map_node\n", Ps_LOG_WARING);
		return;
	}
	assert(_node->map);
	if(_node->map->free_key)
		_node->map->free_key(_node->key);
	if(_node->map->free_value)
		_node->map->free_value(_node->value);
	Ps_Free(_node);
}

static int
compare_map_node(void *node1, void *node2)
{
	struct map_node *_node1 = (struct map_node*)node1;
	struct map_node *_node2 = (struct map_node*)node2;
	int (*compare_func)(void*, void*);
	assert(_node1);
	assert(_node2);
	//assert(_node1->map);
	compare_func = _node1->map->compare_func;
	return compare_func(_node1->key, _node2->key);
}


Map Ps_CreateMap(void *key, void *value, int (*hash_func)(void*),
	void* (*malloc_key)(void*), void* (*malloc_value)(void*), 
	void (*free_key)(void*), void (*free_value)(void*), 
	int (*compare_func)(void *k1, void *k2))
{
	struct Map *map = (struct Map*)Ps_Malloc(sizeof(struct Map));
	struct map_node* node;
	if(map == NULL)
	{
		Ps_Log("Malloc memery for Map failed\n", Ps_LOG_WARING);
		return NULL;
	}
	map->hash_func = hash_func;
	map->malloc_key = malloc_key;
	map->malloc_value = malloc_value;
	map->free_key = free_key;
	map->free_value = free_value;
	map->compare_func = compare_func;

	node = create_Map_node(key, value, map);
	map->tree = Ps_CreateRB((void*)node, compare_map_node, NULL, clear_map_node);
	if(map->tree == NULL)
	{
		Ps_Free(map);
		return NULL;
	}
	return map;
}

BOOL
Ps_MapInsert(Map map, void *key, void *value)
{
	struct map_node *node;
	struct map_node *node2;
	void *_value;
	node = create_Map_node(key, value, map);
	if(node == NULL)
	{
		Ps_Log("MALLOC memser for struct map_node failed\n", Ps_LOG_WARING);
		return FALSE;
	}

	if(map == NULL)
	{
		Ps_Log("Want to insert into Null Map\n", Ps_LOG_WARING);
		return FALSE;
	}
	/*如果map中已存在相应的key的节点*/
	if((node2 = Ps_RBFind(map->tree, node)))
	{
		_value = node->value;
		if(map->free_value)
			map->free_value(_value);
		_value = value;
		if(map->malloc_value)
		{
			_value = map->malloc_value(value);
		}
		node2->value = _value;
		clear_map_node(node);
		return TRUE;
	}
	else
	{
		return Ps_RBInsert(map->tree, node);
	}

}

void* Ps_MapFind(Map map, void *key)
{
	struct map_node *node;
	struct map_node *node2;
	if(map == NULL)
	{
		Ps_Log("Want to find key value in NULL Map\n", Ps_LOG_WARING);
		return NULL;
	}
	node = create_Map_node(key, NULL, map);
	if(node == NULL)
	{
		return NULL;
	}
	node2 = (struct map_node*)Ps_RBFind(map->tree, (void*)node);
	if(node2)

	{
		return node2->value;
	}
	return NULL;
}

BOOL 
Ps_MapDel(Map map, void *key)
{
	struct map_node *node;
	BOOL ret;
	if(map == NULL)
	{
		Ps_Log("Want to del from NULL map\n", Ps_LOG_WARING);
		return FALSE;
	}
	node = create_Map_node(key, NULL, map);
	if(node == NULL)
	{
		return FALSE;
	}
	ret = Ps_RBDel(map->tree, node);
	Ps_Free(node);
	return ret;
}

static void* 
malloc_str(void *str)
{
	int len;
	char *des;
	if(str == NULL)
		return NULL;
	len = strlen((char*)str);
	des = (char*)Ps_Malloc(len + 1);
	memcpy(des, str, len + 1);
	return des;
}

static void
free_str(void *str)
{
	if(str == NULL)
		return;
	Ps_Free(str);
}	

static int 
compare_str(void *str1, void *str2)
{
	assert(str1);
	assert(str2);
	return strcmp(str1, str2);
}

static int 
hash_str(void *str)
{
	int ret;
	int i;
	int len;
	char *_str = (char*)str;
	if(str == NULL)
		return 0;
	len = strlen(_str);
	ret = _str[0];
	for(i = 1; i < len; i ++)
	{
		ret ^= (int)_str[i];
	}
	return ret & Ps_MAP_MASK;
}

Map Ps_CreateMapStr(char *key, char *value)
{	
	return Ps_CreateMap((void*)key, (void*)value, hash_str, 
		malloc_str, malloc_str, free_str, free_str, compare_str);
}

BOOL Ps_MapInsertStr(Map map, char *key, char *value)
{
	return Ps_MapInsert(map, (void*)key, (void*)value);
}

char* Ps_MapFindStr(Map map, char *key)
{
	return (char*)Ps_MapFind(map, (void*)key);
}

BOOL Ps_MapDelStr(Map map, char *key)
{
	return Ps_MapDel(map, (void*)key);
}


#ifdef MAP_DEBUG
#include "../include/init_clear.h"
#define TEST_LEN 256

char buffer[20];

void str(int num)
{
	int i = 0;
	int bit;
	for(; num != 0; i ++)
	{
		bit = num % 10;
		buffer[i] = bit + '0';
		num /= 10;
	}
	buffer[i] = '0';
}

int main(int argc, char **argv)
{
	Ps_INIT(NULL, NULL, NULL);
	Map map = Ps_CreateMapStr("2", "2");
	int i = 0;
	for(i = 1; i < TEST_LEN; i ++)
	{
		str(i);
		Ps_MapInsertStr(map, buffer, buffer);
	}
	printf("%s\n", Ps_MapFindStr(map, "1"));
	printf("%s\n", Ps_MapFindStr(map, "234"));
	Ps_CLEAR();
}
#endif

