#ifndef _THREADMUTEX_H
#define _THREADMUTEX_H

#include <pthread.h>

class ThreadMutex{
public:
    static int ThreadMutex_Init(pthread_mutex_t *mtx); 
    static int ThreadMutex_Destroy(pthread_mutex_t *mtx);
    static int ThreadMutex_Lock(pthread_mutex_t *mtx);
    static int ThreadMutex_Unlock(pthread_mutex_t *mtx);
};

#endif
