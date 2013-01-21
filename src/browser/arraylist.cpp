/**
 *  g++ -o __arraylist__ arraylist.cpp MM.cpp ../lib/mm.c ../util/log.c -lpython2.6 -lcurl
 */ 
#include "arraylist.h"

/*template <class T>
ArrayList<T>::ArrayList() :
    size(0), capacipy(FIRST_SIZE)
{
    values = Ps_New<T>(FIRST_SIZE);
}

template <class T>
ArrayList<T>::ArrayList(T value) :
    size(1), capacipy(FIRST_SIZE)
{
    values = Ps_New<T>(FIRST_SIZE);
    values[0] = value;
}

template <class T>
ArrayList<T>::ArrayList(ArrayList &a)
{
    size = a.size;
    capacipy = a.capacipy;
    values  = Ps_New<T>(capacipy);
    memcpy(values, a.values, sizeof(T) * size);
}

template <class T>
ArrayList<T>::~ArrayList()
{
    Ps_Delete(values);
}

template <class T>
bool ArrayList<T>::at(int location, T value)
{
    T* temp;
    if(location < 0 || location > size)
        return false;
    if(location == capacipy)
    {
        temp = Ps_New<T>(ENLARGE_FACTOR * capacipy);
        memcpy(temp, values, sizeof(T) * capacipy);
        capacipy *= ENLARGE_FACTOR;
        values = temp;

    }
    if(location == size)
        size ++;
    values[location] = value;
    return true;
}

template <class T>
bool ArrayList<T>::at(int location, T *ret)
{
    if(location < 0 || location >= size)
    {
        ret = NULL;
        return false;
    }
    *ret = values[location];
    return true;
}

template <class T>
T
ArrayList<T>::at(int location)
{
    return values[location];
}

template <class T>
bool
ArrayList<T>::insert(int location, T value)
{
    T *temp;
    int i = size - 1;
    if(location < 0 || location > size)
    {
        return false;
    }
    if(size >= capacipy)
    {
        temp = Ps_New<T>(capacipy * ENLARGE_FACTOR);
        memcpy(temp, values, sizeof(T) * capacipy);
        capacipy *= ENLARGE_FACTOR;
        Ps_Delete(values);
        values = temp;
    }
    for(; i >= location; i --)
    {
        values[i + 1] = values[i];
    }
    values[location] = value;
    size ++;
    return true;
}

template <class T>
bool
ArrayList<T>::del(int location)
{
    int i;
    if(location < 0 || location >= size)
    {
        return false;
    }
    if(location == size - 1)
    {
        size --;
        return true;
    }
    for(i = location; i < size - 1; i ++)
    {
        values[i] = values[i + 1];
    }
    size --;
    return true;
}

template <class T>
int 
ArrayList<T>::get_size() const
{
    return this->size;
}

template <class T>
void ArrayList<T>::insert_head(T value)
{
    (void)insert(0, value);
}

template <class T>
void
ArrayList<T>::append(T value)
{
    insert(size, value);
}

template <class T>
int
ArrayList<T>::find(T value)
{
    int i = 0;
    for(; i < size; i ++)
    {
        if(values[i] == value)
            return i;
    }
    return -1;
}

template <class T>
void
ArrayList<T>::clear()
{
    size = 0;
}

#ifdef ARRAYLIST_DEBUG
#include <iostream>
using namespace std;

template <class T>
void show_info(ArrayList<T> al)
{
    int i = 0;
    for(; i < al.get_size(); i ++)
    {
        cout << al.at(i) << " ";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    ArrayList<int> ints;
    ints.at(0, 12);
    show_info<int>(ints);
    ints.append(34);
    show_info<int>(ints);
    ints.at(0, 43);
    show_info<int>(ints);
    ints.insert_head(42);
    show_info<int>(ints);
    ints.append(32);
    show_info<int>(ints);
    ints.del(0);
    show_info(ints);
    ints.del(3);
    show_info(ints);
    ints.del(2);
    show_info(ints);
    for(int i = 10; i < 25; i ++)
        ints.append(i);
    show_info(ints);
    for(int i = ints.get_size(); i < 45; i ++)
        ints.insert(i - 5, i);
    show_info(ints);
    cout << ints.find(42) << endl;
    ints.clear();
    show_info(ints);
    cout << ints.find(42) << endl;
}
#endif*/


