/**
 * Ps's exception system
 * test: g++ -o __exception__ exception.cpp
 **/
#include "../include/exception.h"

/*******Ps_Exception*************/
Ps_Exception::Ps_Exception()
{
    value = (void*)0;
}

Ps_Exception::Ps_Exception(void *value)
{
    this->value = value;
}

Ps_Exception::~Ps_Exception()
{

}

const char*
Ps_Exception::what() const throw()
{
    return Ps_COMMON_EXCEPTION_NAME;
}

void*
Ps_Exception::get_value()
{
    return value;
}

/********Ps_NoMemeryException*********/
Ps_NoMemeryException::Ps_NoMemeryException() :
    Ps_Exception()
{

}

Ps_NoMemeryException::Ps_NoMemeryException(void *value) :
    Ps_Exception(value)
{

}

Ps_NoMemeryException::~Ps_NoMemeryException()
{

}

const char*
Ps_NoMemeryException::what() const throw()
{
    return Ps_NOMEMERY_EXCEPTION_NAME;
}

/********Ps_PointerException************/
Ps_PointerException::Ps_PointerException() :
    Ps_Exception()
{

}

Ps_PointerException::Ps_PointerException(void *value) :
    Ps_Exception(value)
{

}

Ps_PointerException::~Ps_PointerException()
{

}

const char*
Ps_PointerException::what() const throw()
{
    return Ps_POINTER_EXCEPION_NAME;
}
/***********Ps_OutOfIndexException****/
Ps_OutOfIndexException::Ps_OutOfIndexException() :
    Ps_Exception()
{

}

Ps_OutOfIndexException::Ps_OutOfIndexException(void *value) :
    Ps_Exception(value)
{

}

Ps_OutOfIndexException::~Ps_OutOfIndexException()
{

}

const char*
Ps_OutOfIndexException::what() const throw()
{
    return Ps_OUTOFINDEX_EXCEPTION_NAME;
}

/*************************************
***********TEST*********************
***********************************/
#ifdef EXCEPION_DEBUG
#include <iostream>
char *info = (char*)"Hello I am exception";

void test_exception1()
{
    throw Ps_Exception();
}

void test_exception2()
{
    throw Ps_Exception((void*)info);
}

void test_nomem_exception()
{
    throw Ps_NoMemeryException();
}

void test_pointer_exception()
{
    throw Ps_PointerException();
}

void test_outofindex_exception()
{
    throw Ps_OutOfIndexException();
}

int main(int argc, char **argv)
{
    /******NO 1:test Ps_Exception*****/
    try
    {
        test_exception1();
    }
        catch(Ps_Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    /******NO 2:test Ps_Exception*****/
    try
    {
        test_exception2();
    }
        catch(Ps_Exception &e)
    {
        std::cout <<(char*)e.get_value() << std::endl;
    }
    /*******************************/
    try
    {
        test_nomem_exception();
    }
        catch(Ps_Exception &e)
    {
        std::cout << e.get_value() << std::endl;
        std::cout << e.what() << std::endl;
    }
    /*****************************/
    try
    {
        test_pointer_exception();
    }
        catch(Ps_Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    /****************************/
    try
    {
        test_outofindex_exception();
    }
    catch(Ps_Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

#endif




















