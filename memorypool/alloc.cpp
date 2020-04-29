#include "alloc.h"

void (* alloc::alloc_handler)() = 0;

void *alloc::retry_malloc(size_t n)
{
    void (*handler)();
    void *result;

    for(;;){
        handler = alloc_handler;
        if(0 == handler){ /*TODO*/ }
        (*handler)();
        result = malloc(n);
        if(result) return result; 
    }
}

void *alloc::retry_realloc(void *p, size_t n)
{
    void (*handler)();
    void *result;

    for(;;){
        handler = alloc_handler;
        if(0 == handler){ /*TODO*/ }
        (*handler)();
        result = realloc(p, n);
        if(result) return result;
    }
}

void *alloc::allocate(size_t n)
{
    void *result = malloc(n);
    if(0 == result){
        result = retry_malloc(n);
    }
    return result;
}

void *alloc::deallocate(void *p, size_t n)
{
    free(p);
}

void *alloc::reallocate(void *p, size_t nw_sz)
{
    void *result = realloc(p, nw_sz);
    if(0 == result){
        result = retry_realloc(p, nw_sz);
    }
    return result;
}


void (* alloc::set_alloc_handler(void (*func)()))()
{
    void (*old)() = alloc_handler;
    alloc_handler = func;
    return old;
}
