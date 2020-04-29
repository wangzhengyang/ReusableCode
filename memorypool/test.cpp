#include <iostream>
#include "palloc.h"


int main(void)
{
    char *p[100];
    for(int i = 0; i < 100; i++){
        p[i] = (char*)malloc_palloc::allocate(4 * i);
        if(p[i] != 0){
            std::cout << "alloc success:" << 4 * i << std::endl;
        }else{
            std::cout << "alloc failed" << std::endl;
        }
    }
    for(int i = 0; i < 100; i++){
        if(p[i] != 0) malloc_palloc::deallocate(p[i], 4 * i + 1);
    }
    return 0;
}
