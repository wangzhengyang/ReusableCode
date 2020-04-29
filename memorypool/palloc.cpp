#include "palloc.h"

char *palloc::start_free = 0;
char *palloc::end_free = 0;
size_t palloc::heapsize = 0;

typename palloc::Obj* volatile palloc::free_list[palloc::_NFREELISTS] = {\
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

size_t palloc::round_up(size_t bytes)
{
    return ((bytes + (size_t)_ALIGN - 1) &~((size_t)_ALIGN - 1));
}

size_t palloc::freelist_index(size_t bytes)
{
    return ((bytes + (size_t)_ALIGN - 1)/((size_t)_ALIGN - 1));
}

void *palloc::chunk_alloc(size_t size, int &objs)
{
    char *result;
    size_t total_bytes = size * objs;
    size_t bytes_left = end_free - start_free;
    if(bytes_left >= total_bytes){
        result = start_free;
        return (result);
    }else if(bytes_left >= size){
        objs = (int)(bytes_left/size);
        total_bytes = size * objs;
        result = start_free;
        start_free += total_bytes;
        return (result);
    }else{
        size_t bytes_to_get = 2 * total_bytes + round_up(heapsize >> 4);
        if(bytes_left > 0){
            Obj* volatile* my_free_list = free_list + freelist_index(bytes_left);
            ((Obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (Obj*)start_free;
        }
        start_free = (char*)malloc(bytes_to_get);
        if(0 == start_free){
            size_t i;
            Obj* volatile* e_free_list;
            Obj* p;
            for(i = size; i <= (size_t)_MAX_BYTES; i += (size_t)_ALIGN){
                e_free_list = free_list + freelist_index(i);
                p = *e_free_list;
                if(0 != p){
                    *e_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + i;
                    return (chunk_alloc(size, objs));
                }
            }
            end_free = 0;
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heapsize += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return (chunk_alloc(size, objs));
    }
}

void *palloc::refill(size_t n)
{
    int objs = 20;
    void *chunk = chunk_alloc(n, objs);
    Obj* volatile* my_free_list;
    Obj* result;
    Obj* current_obj;
    Obj* next_obj;
    int i;

    if(1 == objs)return (chunk);
    my_free_list = free_list + freelist_index(n);
    result = (Obj*)chunk;
    *my_free_list = next_obj = (Obj*)((char*)chunk + n);
    for(i = 1; ; i++){
        current_obj = next_obj;
        next_obj = (Obj*)((char*)next_obj + n);
        if(objs - 1 == i){
            current_obj->free_list_link = 0;
            break;
        }else{
            current_obj->free_list_link = next_obj;
        }
    }
    return (result);
}

void *palloc::allocate(size_t n)
{
    void *ret = 0;
    if(n == 0) return ret;

    if(n > (size_t)_MAX_BYTES){
        ret = malloc_alloc::allocate(n);
    }else{
        Obj* volatile* my_free_list = free_list + freelist_index(n);
        Obj* result = *my_free_list;
        if(0 == result){
            ret = refill(round_up(n));
        }else{
            *my_free_list = result->free_list_link;
            ret = result;
        }
    }
    return ret;
}

void *palloc::deallocate(void *p, size_t n)
{
    if(n > (size_t)_MAX_BYTES){
        malloc_alloc::deallocate(p, n);
    }else{
        Obj* volatile* my_free_list = free_list + freelist_index(n);
        Obj* q = (Obj*)p;
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }
}

void *palloc::reallocate(void *p, size_t old_sz, size_t nw_sz)
{
    void *result;
    size_t copy_sz;
    if(old_sz > (size_t)_MAX_BYTES && nw_sz > (size_t)_MAX_BYTES){
        return (realloc(p, nw_sz));
    }
    if(round_up(old_sz) == round_up(nw_sz)) return (p);
    result = allocate(nw_sz);
    copy_sz = nw_sz > old_sz ? old_sz : nw_sz;
    memcpy(result, p, copy_sz);
    deallocate(p, old_sz);
    return (result);
}

