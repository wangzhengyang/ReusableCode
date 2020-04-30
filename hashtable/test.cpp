#include <iostream>
#include "hashtable.h"


int main(void)
{
    Hashtable hashmap(17);
    Hashtable_Entry elem1(1);
    Hashtable_Entry elem2(19);
    Hashtable_Entry elem3(84);
    Hashtable_Entry elem4(192);
    Hashtable_Entry elem5(34);
    Hashtable_Entry elem6(213);
    Hashtable_Entry elem8(1023);
    Hashtable_Entry elem9(132);

    hashmap.addElem(&elem1);
    hashmap.addElem(&elem8);
    hashmap.addElem(&elem2);
    hashmap.addElem(&elem3);
    hashmap.addElem(&elem4);
    hashmap.addElem(&elem5);
    hashmap.addElem(&elem6);
    hashmap.addElem(&elem9);


    std::cout << "find elem1" << std::endl;
    Hashtable_Entry *p;
    p = hashmap.find(1);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }
    

    std::cout << "find elem8" << std::endl;
    p = hashmap.find(1023);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }

    std::cout << "find elem5" << std::endl;
    p = hashmap.find(34);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }

    std::cout << "find elem7" << std::endl;
    p = hashmap.find(9999);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }

    std::cout << "find elem6" << std::endl;
    p = hashmap.find(213);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }

    std::cout << "delete elem6" << std::endl;
    hashmap.removeElem(p);

    std::cout << "find elem6" << std::endl;
    p = hashmap.find(213);
    if(p != 0){
        std::cout << "success: " << p->value << std::endl;
    }else{
        std::cout << "failed: " << std::endl;
    }

    return 0;
}
