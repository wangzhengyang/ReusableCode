#include <iostream>
#include <sys/time.h>
#include "timertask.h"
#include "timer.h"

void printfhello(void *arg)
{
    //std::cout << "hello wangzhengyang" << std::endl;
}

void printfshit(void *arg)
{
}

int main(void)
{
    Timer timer;
    TimerTask task1(printfhello, NULL, 1);
//    TimerTask task2(printfshit, NULL, 2000);
    timer.TimertaskRegister(&task1);
//    timer.TimertaskRegister(&task2);
    timer.Start();

    while(1){
        sleep(1);
    }
}
