#include "centerlinear.h"



#ifdef CENTER_LINEAR_DEBUG
int main(int argc, char **argv)
{
    int i = 0;
    bool r;
    CenterLinear<int> cl;
    r = cl.is_null();
    try{
        cl.insert_head(0);
        //std::cout << cl.prev() << std::endl;
        cl.insert_tail(1);
        std::cout << "cl prev " << cl.prev() << std::endl;
        cl.insert_next_head(4);
        std::cout << "cl next " << cl.next() <<std::endl;
        //std::cout << "cl next " << cl.next() <<std::endl;
        cl.insert_head(7);
        std::cout << "cl next " << cl.next() <<std::endl;
        cl.insert_next_head(2);
        cl.insert_prev_head(3);
        cl.insert_tail(5);
        cl.insert_tail(6);
        cl.insert_head(8);
    }
    catch(Ps_Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;

}

#endif
