#include <stdlib.h>
#include "hashtable.h"


Hashtable::Hashtable(int sz):size(sz)
{
    hashmap = (Hashtable_Entry*)malloc(sz * sizeof(Hashtable_Entry));
    if(hashmap == 0) { /*TODO*/ }
    Hashtable_Entry *tmp;
    for(int i = 0; i < sz; i++){
        tmp = &hashmap[i];
        tmp->next = tmp;
        tmp->prev = tmp;
    }

}

Hashtable::~Hashtable()
{
    if(hashmap == 0){ /*TODO*/ }
    free(hashmap);
}

unsigned int Hashtable::generateKey(unsigned int value)
{
    return (value % size);
}

int Hashtable::addElem(Hashtable_Entry *elem)
{
    if(elem == 0 || hashmap == 0 || size == 0) return -1;
    Hashtable_Entry *tmp = &hashmap[generateKey(elem->value)];
    elem->next = tmp->next;
    tmp->next->prev = elem;
    
    tmp->next = elem;
    elem->prev = tmp;
    return 0;
}


int Hashtable::removeElem(Hashtable_Entry *elem)
{
    if(elem == 0 || hashmap == 0 || size == 0) return -1;
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
    return 0;
}


Hashtable_Entry *Hashtable::find(int value)
{
    Hashtable_Entry *tmp = &hashmap[generateKey(value)];
    Hashtable_Entry *p = 0;
    for(p = tmp->next; p != tmp; p = p->next){
        if(p->value == value)
            return p;
    }
    
    return 0;
}

