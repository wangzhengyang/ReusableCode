#include "threadcond.h"

int ThreadCond::ThreadCond_Init(pthread_cond_t *cond)
{
    int err;

    err = pthread_cond_init(cond, NULL);
    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}

int ThreadCond::ThreadCond_Destroy(pthread_cond_t *cond)
{
    int err;

    err = pthread_cond_destroy(cond);
    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}


int ThreadCond::ThreadCond_Signal(pthread_cond_t *cond)
{
    int err;

    err = pthread_cond_signal(cond);
    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}

int ThreadCond::ThreadCond_Wait(pthread_cond_t *cond, pthread_mutex_t *mtx)
{
    int err;

    err = pthread_cond_wait(cond, mtx);
    if(err == 0){
        return 0;
    }

    /*TODO*/
    return -1;
}
