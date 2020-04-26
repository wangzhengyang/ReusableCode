#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <pthread.h>
#include <queue>
#include <signal.h>
#include <unistd.h>

#include "threadmutex.h"
#include "threadcond.h"

typedef void (*taskhandler)(void *arg);

class ThreadTask{
private:
    taskhandler handler;
    void *taskarg;
public:
   explicit ThreadTask(taskhandler hand, void *arg):handler(hand), taskarg(arg){ }
   ~ThreadTask(){ }
   void run(){ handler(taskarg);}
};


class ThreadPool{
private:
    std::queue<ThreadTask*> queue;

    unsigned int threads;
    unsigned int maxqueue;
    unsigned int waiting;

    pthread_mutex_t mtx;
    pthread_cond_t cond;

    static void *ThreadPool_Cycle(void *data);
    static void ThreadPool_exit_handler(void *data);
public: 
    explicit ThreadPool(int nums, int maxq):threads(nums),maxqueue(maxq), waiting(0){ }    
    ~ThreadPool(){ }
    static int ThreadPool_Init(ThreadPool *pool);
    static int ThreadPool_Post(ThreadPool *pool, ThreadTask *ptask);
    static void ThreadPool_Destroy(ThreadPool *pool);
};





#endif
