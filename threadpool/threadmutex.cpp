#include "threadmutex.h"

int ThreadMutex::ThreadMutex_Init(pthread_mutex_t *mtx) 
{
    int err;
    pthread_mutexattr_t attr;

    err = pthread_mutexattr_init(&attr);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_mutex_init(mtx, &attr);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_mutexattr_destroy(&attr);
    if(err != 0){
        /*TODO*/
    }

    return 0;
}

int ThreadMutex::ThreadMutex_Destroy(pthread_mutex_t *mtx)
{
    int err;

    err = pthread_mutex_destroy(mtx);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    
    return 0;
}

int ThreadMutex::ThreadMutex_Lock(pthread_mutex_t *mtx)
{
    int err;
    
    err = pthread_mutex_lock(mtx);
    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}


int ThreadMutex::ThreadMutex_Unlock(pthread_mutex_t *mtx)
{
    int err;

    err = pthread_mutex_unlock(mtx);

    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}
