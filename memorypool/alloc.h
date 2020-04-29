#ifndef _ALLOC_H
#define _ALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

class alloc{
private:
    static void *retry_malloc(size_t n);
    static void *retry_realloc(void *p, size_t n);
    
    static void (*alloc_handler)();
public:
    static void *allocate(size_t n);
    static void *deallocate(void *p, size_t n);
    static void *reallocate(void *p, size_t nw_sz);
    static void (* set_alloc_handler(void (*func)()))();
};

typedef alloc malloc_alloc;

#endif
