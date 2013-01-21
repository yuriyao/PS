/**
 *  用于C++的内存管理
 *  调试选项：g++ -o __MM__ MM.cpp ../lib/mm.c -lpython2.6
 */ 
#include "MM.h"

/*template <class T>
T*
Ps_New()
{
    int size = sizeof(T);
    void *p = Ps_Malloc(size);
    return (T*)p;
}

template <class T>
T*
Ps_New(int number)
{
    int size = sizeof(T) * number;
    void *p = Ps_Malloc(size);
    return (T*)p;
}*/

void Ps_Delete(void *p)
{
    Ps_Free(p);
}

#ifdef MM_CPP_DEBUG
#include <iostream>
#include <string.h>
using namespace std;
int main(int argc, char **argv)
{
    int *i = Ps_New<int>();
    string *s = Ps_New<string>();
    new(s) string("hello world");
    *i = 10;
    cout << *i << " " << i << "\n" << *s << endl;
    Ps_Delete(s);
    Ps_Delete(i);
}
#endif
