#ifndef _THREADCOND_H
#define _THREADCOND_H

#include <pthread.h>

class ThreadCond{
public:
    static int ThreadCond_Init(pthread_cond_t *cond);
    static int ThreadCond_Destroy(pthread_cond_t *cond);
    static int ThreadCond_Signal(pthread_cond_t *cond);
    static int ThreadCond_Wait(pthread_cond_t *cond, pthread_mutex_t *mtx);
};

#endif

