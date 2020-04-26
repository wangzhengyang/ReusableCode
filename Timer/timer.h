#ifndef _TIMER_H
#define _TIMER_H

#include <pthread.h>
#include <list>
#include <unistd.h>
#include "timertask.h"

class Timer{
private:
    std::list<TimerTask*> tasklist;

    pthread_rwlock_t rwlock;

    static void *Run(void*);
public:
    Timer();
    ~Timer();
    int TimertaskRegister(TimerTask* task);
    int TimertaskUnregister(TimerTask *task);
    int Start(); 

};

#endif
