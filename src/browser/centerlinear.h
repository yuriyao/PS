/**
 * Center linear
 * test:g++ -o __cl__ centerlinear.cpp ../lib/exception.cpp MM.cpp ../lib/mm.c
 */
#ifndef CENTERLINEAR_H
#define CENTERLINEAR_H
#include "MM.h"
#include "../include/exception.h"

#define CENTER_LINEAR_DEBUG

#ifdef CENTER_LINEAR_DEBUG
#include <iostream>
#endif

template <class T>
class CenterLinearNode
{
public:
    CenterLinearNode(T value) :
        prev(NULL), next(NULL), value(value)
    {}

    CenterLinearNode *prev, *next;
    T value;
};

template <class T>
class CenterLinear
{
public:
    CenterLinear();
    CenterLinear(T value);
    CenterLinear(CenterLinear &other);
    ~CenterLinear();

    bool have_prev();
    bool have_next();
    bool is_null();

    /**
     * @brief Insert into the current behind
     *        and move to it.
     * @param value
     */
    void insert_next_head(T value);

    /**
     * @brief Insert into the current forhead
     *        and move to it.
     * @param value
     */
    void insert_prev_head(T value);

    /**
     * @brief Insert into the head
     *        and move to it.
     * @param value
     */
    void insert_head(T value);

    /**
     * @brief Insert into the tail
     *        and move to it.
     * @param value
     */
    void insert_tail(T value);

    /*
     * Get current value
     * if current doesn't exit
     * throw exception(Ps_OutOfIndexException)
     */
    T get_current();

    /*
     * Go to prev value and return it
     * if next doesn't exist, will throw
     * exception(Ps_OutOfIndexException)
     */
    T prev();

    /*
     * Go to next value and return it
     * if next doesn't exist, will throw
     * exception(Ps_OutOfIndexException)
     */
    T next();

private:
    CenterLinearNode<T> *center;
};

template <class T>
CenterLinear<T>::CenterLinear() :
    center(NULL)
{

}

template <class T>
CenterLinear<T>::CenterLinear(T value)
{
    center = Ps_New<CenterLinearNode<T> >();
    new(center) CenterLinearNode<T>(value);
}

template <class T>
CenterLinear<T>::CenterLinear(CenterLinear &other)
{
    Ps_Log("It's dangerout to copy the center linear\n", Ps_LOG_WARING);
    *this = other;
}

template <class T>
CenterLinear<T>::~CenterLinear()
{
    if(!center)
        return;
    CenterLinearNode<T> *t = center->prev;
    CenterLinearNode<T> *v;
    while(t)
    {
        v = t->prev;
#ifdef CENTER_LINEAR_DEBUG
        std::cout << t->value << std::endl;
#endif
        Ps_Free(t);
        t = v;
    }
    while(center)
    {
        v = center->next;
#ifdef CENTER_LINEAR_DEBUG
        std::cout << center->value << std::endl;
#endif
        Ps_Free(center);
        center = v;
    }
}

template <class T>
bool
CenterLinear<T>::have_prev()
{
    return (center && (center->prev));
}

template <class T>
bool
CenterLinear<T>::have_next()
{
    return (center && (center->next));
}

template <class T>
bool
CenterLinear<T>::is_null()
{
    return (center == NULL);
}

template <class T>
void
CenterLinear<T>::insert_next_head(T value)
{
    CenterLinearNode<T> *t= Ps_New<CenterLinearNode <T> >();
    new(t) CenterLinearNode<T>(value);
    if(!center)
    {
        center = t;
        return;
    }
    t->next = center->next;
    t->prev = center;
    if(center->next)
        center->next->prev = t;
    center->next = t;
    center = t;
}

template <class T>
void
CenterLinear<T>::insert_prev_head(T value)
{
    CenterLinearNode<T> *t = Ps_New<CenterLinearNode <T> >();
    new(t) CenterLinearNode<T>(value);
    if(!center)
    {
        center = t;
        return;
    }
    if(center->prev)
        center->prev->next = t;
    t->prev = center->prev;
    t->next = center;
    center->prev = t;
    center = t;
}

template <class T>
void
CenterLinear<T>::insert_head(T value)
{
    CenterLinearNode<T> *t = Ps_New<CenterLinearNode <T> >();
    CenterLinearNode<T> *v = center;
    new(t) CenterLinearNode<T>(value);
    if(!center)
    {
        center = t;
        return;
    }
    while(v->prev)
    {
        v = v->prev;
    }
    v->prev = t;
    t->next = v;
    center = t;
}

template <class T>
void
CenterLinear<T>::insert_tail(T value)
{
    CenterLinearNode<T> *t = Ps_New<CenterLinearNode <T> >();
    CenterLinearNode<T> *v = center;
    new(t) CenterLinearNode<T>(value);
    if(!center)
    {
        center = t;
        return;
    }
    while(v->next)
    {
        v = v->next;
    }
    v->next = t;
    t->prev = v;
    center = t;
}

template <class T>
T
CenterLinear<T>::get_current()
{
    if(!center)
        throw Ps_OutOfIndexException();
    return center->value;
}

template <class T>
T
CenterLinear<T>::prev()
{
    if(!center || !center->prev)
        throw Ps_OutOfIndexException();
    center = center->prev;
    return center->value;
}

template <class T>
T
CenterLinear<T>::next()
{
    if(!center || !center->next)
        throw Ps_OutOfIndexException();
    center = center->next;
    return center->value;
}

#endif // CENTERLINEAR_H


















