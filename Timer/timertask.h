#ifndef _TIMERTASK_H
#define _TIMERTASK_H

typedef void (*Timer_Task)(void *param);

class TimerTask{
private:
    Timer_Task task;
    void *param;
    unsigned int cycle;
    unsigned int curr;
public:
    TimerTask(Timer_Task tsk, void *pm, unsigned int cyc):task(tsk),param(pm),cycle(cyc),curr(0){ }
    ~TimerTask(){ }

    void Update();
};



#endif
