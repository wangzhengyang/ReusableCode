#include <iostream>
#include <sys/time.h>
#include <sched.h>
#include "timer.h"


Timer::Timer()
{
    int err;
    err = pthread_rwlock_init(&rwlock, NULL);
    if(err != 0){
        /*TODO*/
    }
}


Timer::~Timer()
{
    int err = 0;
    err = pthread_rwlock_wrlock(&rwlock);
    if(err != 0){
        /*TODO*/
    }
    tasklist.clear();
    err = pthread_rwlock_unlock(&rwlock);
    if(err != 0){
        /*TODO*/
    }
    err = pthread_rwlock_destroy(&rwlock);
    if(err != 0){
        /*TODO*/
    }

}


int Timer::TimertaskRegister(TimerTask *ptask)
{
    int err = 0;
    err = pthread_rwlock_wrlock(&rwlock);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    tasklist.push_back(ptask);
    err = pthread_rwlock_unlock(&rwlock);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    return 0;
}

int Timer::TimertaskUnregister(TimerTask *ptask)
{
    int err = 0;
    err = pthread_rwlock_wrlock(&rwlock);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    tasklist.remove(ptask);
    err = pthread_rwlock_unlock(&rwlock);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    return 0;
}

int Timer::Start()
{
    pthread_t tid;
    pthread_attr_t attr;
    struct sched_param params;
    int err;

    err = pthread_attr_init(&attr);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    params.sched_priority = 10;
    err = pthread_attr_setschedparam(&attr, &params);
    if(err != 0){
        /*TODO*/
        return -1;
    }

    err = pthread_create(&tid, &attr, Run, this);
    if(err != 0){
        /*TODO*/
        return -1;
    }
    return 0;
}

void *Timer::Run(void *arg)
{
    Timer *ptimer = (Timer*)arg;
    std::list<TimerTask*>::iterator p;
    int err = 0;
    struct timeval interval;
    struct timeval old;
    struct timeval nw;
    gettimeofday(&old, NULL);
    for(;;){
        gettimeofday(&nw, NULL);
        std::cout << nw.tv_sec - old.tv_sec << ":" << (nw.tv_usec - old.tv_usec)/1000 << std::endl;
        old.tv_sec = nw.tv_sec;
        old.tv_usec = nw.tv_usec;
        err = pthread_rwlock_rdlock(&ptimer->rwlock);
        if(err != 0){
            /*TODO*/
        }
        for(p = ptimer->tasklist.begin(); p != ptimer->tasklist.end(); ++p){
            (*p)->Update();
        }
        err = pthread_rwlock_unlock(&ptimer->rwlock);
        if(err != 0){
            /*TODO*/
        }
        usleep(100000);

    }

}
