#ifndef MM_CPP_H
#define MM_CPP_H
#include "../include/mm.h"
#include "../include/log.h"
#include "../include/exception.h"
//#define MM_CPP_DEBUG


template <class T>
T*
Ps_New()
{
    int size = sizeof(T);
    void *p = Ps_Malloc(size);
    if(!p)
        throw Ps_NoMemeryException();
    return (T*)p;
}

template <class T>
T*
Ps_New(int number)
{
    int size = sizeof(T) * number;
    void *p = Ps_Malloc(size);
    if(!p)
        throw Ps_NoMemeryException();
    return (T*)p;
}

void Ps_Delete(void *p);


#endif // MM_H
