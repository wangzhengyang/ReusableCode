#ifndef _PALLOC_H
#define _PALLOC_H

#include "alloc.h"

class palloc{
private:
    enum{ _ALIGN = 8 };
    enum{ _MAX_BYTES = 128 };
    enum{ _NFREELISTS = 16 };

    union Obj{ 
        union Obj *free_list_link; 
        char data[1];
    };

    static Obj* volatile free_list[_NFREELISTS];

    static char *start_free;
    static char *end_free;
    static size_t heapsize;

    static size_t round_up(size_t bytes);
    static size_t freelist_index(size_t bytes);
    static void *refill(size_t n);
    static void *chunk_alloc(size_t size, int &objs);

public:
    static void *allocate(size_t n);
    static void *deallocate(void *p, size_t n);
    static void *reallocate(void *p, size_t old_sz, size_t new_sz);
};

typedef palloc malloc_palloc;

#endif

