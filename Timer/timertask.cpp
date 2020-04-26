#include "timertask.h"


void TimerTask::Update()
{
    if(curr == 0){
        curr = cycle;
        task(param);
    }
    curr--;
}
