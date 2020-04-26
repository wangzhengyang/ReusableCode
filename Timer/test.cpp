#include <iostream>
#include <sys/time.h>
#include "timertask.h"
#include "timer.h"

void printfhello(void *arg)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    std::cout << t.tv_sec << ":" << t.tv_usec << std::endl;
}

void printfshit(void *arg)
{
}

int main(void)
{
    Timer timer;
    TimerTask task1(printfhello, NULL, 100);
//    TimerTask task2(printfshit, NULL, 2000);
    timer.TimertaskRegister(&task1);
//    timer.TimertaskRegister(&task2);
    timer.Start();

    while(1){
        sleep(1);
    }
}
