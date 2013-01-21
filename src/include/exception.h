#ifndef EXCEPTION_H
#define EXCEPTION_H

#define Ps_COMMON_EXCEPTION_NAME "Ps Common exception"
#define Ps_NOMEMERY_EXCEPTION_NAME "Ps detected no enough memory"
#define Ps_POINTER_EXCEPION_NAME "Ps detected using error pointer"
#define Ps_OUTOFINDEX_EXCEPTION_NAME "Ps detected out of index"

class Ps_Exception
{
public:
    Ps_Exception();
    Ps_Exception(void *value);
    virtual ~Ps_Exception();
    virtual const char* what() const throw();
    virtual void* get_value();
private:
    void *value;
};

class Ps_NoMemeryException : public Ps_Exception
{
public:
    Ps_NoMemeryException();
    Ps_NoMemeryException(void *value);
    ~Ps_NoMemeryException();
    virtual const char* what() const throw();
};

class Ps_PointerException : public Ps_Exception
{
public:
    Ps_PointerException();
    Ps_PointerException(void *value);
    virtual ~Ps_PointerException();
    virtual const char* what() const throw();
};

class Ps_OutOfIndexException : public Ps_Exception
{
public:
    Ps_OutOfIndexException();
    Ps_OutOfIndexException(void *value);
    virtual ~Ps_OutOfIndexException();
    virtual const char* what() const throw();
};

//#define EXCEPION_DEBUG

#endif // EXCEPTION_H
