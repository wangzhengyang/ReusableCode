#include <stdio.h>
#include "threadpool.h"

ThreadPool pool(12, 10);

void process(void *arg)
{
    int *count = (int*)arg;
    char str[50];
    int len;
    len = sprintf(str, "the count is %d\r\n", *count);
    write(1, str, len);
    
}

void *sendtask(void *arg)
{
    int count = 0;
    ThreadTask task(process, &count);

    for(;;){
        count ++;
        ThreadPool::ThreadPool_Post(&pool, &task); 
        usleep(1000);
    }
}

int main(void )
{
    pthread_t tid;
    pthread_attr_t attr;
    int err;
    ThreadPool::ThreadPool_Init(&pool);

    err = pthread_attr_init(&attr);
    if(err != 0){
       write(1, "attr init failed", 17);
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    err = pthread_create(&tid, &attr, sendtask, NULL);
    if(err != 0){
        write(1, "create sendtask failed", 25);
    } 


    while(1){
        sleep(1);
    }

    return 0;
}
