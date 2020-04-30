#ifndef _HASHTABLE_H
#define _HASHTABLE_H

struct Hashtable_Entry{
    unsigned int value;

    Hashtable_Entry *next;
    Hashtable_Entry *prev;

    Hashtable_Entry(unsigned int val):value(val),next(0),prev(0){ }
}; 


class Hashtable{
private:
    Hashtable_Entry *hashmap;
    unsigned int size;

    unsigned int generateKey(unsigned int value);

public:
    Hashtable(int size);
    ~Hashtable();

    int addElem(Hashtable_Entry *elem);
    int removeElem(Hashtable_Entry *elem);
    Hashtable_Entry *find(int value);
};



#endif
