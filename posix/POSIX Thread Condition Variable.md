# POSIX Thread Condition Variable

[toc]

通常条件变量与互斥量一块使用。条件的测试必须在互斥量的保护下进行

当条件测试失败，线程通常会堵塞并且释放掉互斥量然后等待条件改变

当另外一个线程改变条件时，会发送信号给条件变量，从而引起一个或者多个等待线程执行下面的动作：

1. 苏醒
2. 再次获取互斥量
3. 再次判定条件

条件变量也可用于进程间同步

调度策略决定了堵塞的线程怎样苏醒。默认的调度策略`SCHED_OTHER`不会规定哪个线程先苏醒。如果是使用`SCHED_FIFO`或者`SCHED_RR`实时调度策略，则线程是按照优先等级苏醒

## Initializing a Condition Variable Attribute(初始化条件变量属性)

### Syntax

```c
int pthread_condattr_init(pthread_condattr_t *cattr);
```

默认属性`PTHREAD_PROCESS_PRIVATE`,条件变量属性在重复使用前，必须通过`pthread_condattr_destroy`释放掉，否则会引起内存泄露

### Return values

1. `0` means the call complete successfully
2. `ENOMEM` means insufficient memory allocated to initialize the thread attributes object
3. `EINVAL` means the value specified by `cattr` is invalid

## Removing a Condition Variable Attribute(销毁条件变量属性)

### Syntax

```c
int pthread_condattr_destroy(pthread_condattr_t *cattr);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `cattr` is invalid

## Setting the Scope of a Condition Variable(设置条件变量作用范围)

### Syntax

```c
int pthread_condattr_setpshared(pthread_condattr_t *cattr, int pshared);
```

### `pshared`

1. `PTHREAD_PROCESS_SHARED`
2. `PTHREAD_PROCESS_PRIVATE`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `cattr` is invalid, or the `pshared` value is invalid

## Getting the Scope of a Condition Variable(获取条件变量作用范围)

### Syntax

```c
int pthread_condattr_getpshared(const pthread_condattr_t *restrict cattr, int *restrict pshared);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `cattr` is invalid

## Setting the Clock Selection Condition Variable(设置条件变量时钟选择-暂不使用)

### Syntax

```c
int pthread_condattr_setclock(pthread_condattr_t *restrict attr, clockid_t *restrict clock_id);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` menas the value specified by `clock_id` does not refer to a known clock, or is a CPU-time clock

## Geting the Clock Selection Condition Variable(获取条件变量时钟选择-暂不使用)

### Syntax

```c
int pthread_condattr_getclock(const pthread_condattr_t *restrict attr, clockid_t *restrict clock_id);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `attr` is invalid

## Initializing a Condition Variable(初始化条件变量)

### Syntax

```c
int pthread_cond_init(pthread_cond_t *restrict cv, const pthread_condattr_t *restrict cattr);
or
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `cattr` is invalid
3. `EBUSY` means the condition variable is being used
4. `EAGAIN` means the necessary resources are not available

## Blocking on a Condition Variable(堵塞条件变量)

### Syntax

```c
int pthread_cond_wait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mutex);
```

堵塞的线程通过以下方式可以被唤醒：

1. `pthread_cond_signal()`以及`pthread_cond_broadcast()`
2. 被传递过来的一个信号打断

函数会一直堵塞直到条件信号发送过来，函数在堵塞之前会自动释放掉互斥量，在返回之前自动获得互斥量

`pthread_cond_wait()`是一个可以被终止的点，在被终止后会继续持有该锁，可以通过清理函数来释放锁

**导致堵塞的条件点必须再次测试在调用该函数之前，因为条件会该改变如果在线程获得互斥量之前，强烈建议使用`while()`循环去测试条件在使用该函数之前**

```c
pthread_mutex_lock();
	while(condition_is_false)
        	pthread_cond_wait();
pthread_mutex_unlock();
```



### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `cv` or `mp` is invalid

## Unblocking One Thread(释放条件变量)

### Syntax

```c
int pthread_cond_signal(pthread_cont_t *cv);
```

如果没有线程因为条件变量堵塞，则调用`pthread_cond_signal`不会有任何影响

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `cv` points to an illegal address

## Blocking Until a Specified Time(堵塞特定时间)

### Syntax

```c
#include <time.h>
int phtread_cond_timedwait(pthread_condt_t *restrict cv, pthread_mutex_t *restrict mp, const struct timespec *restrict abstime);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `cv`,`mp`,or `abstime` points to an illegal address, different mutexes were supplied for concurrent `pthread_cond_timedwait()` operations on the same condition variable
3. `ETIMEDOUT` means the time specified by `abstime` has passed
4. `EPERM` means the mutex was not owned by the current thread at the time of  the call

## Blocking For a Specified Interval(堵塞特定周期)

### Syntax

```c
int pthread_cond_reltimedwait_np(pthread_cond_t *cv, pthread_mutex_t *mp, const struct timespec *reltime);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `reltime` is invalid
3. `ETIMEDOUT` means the time interval specified by `reltime` has passed

## Unblocking All Threads(释放所有条件变量)

### Syntax

```c
int pthread_cond_broadcast(pthread_cond_t *cv);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `cv` points to an illegal address

## Destroying the Condition Variable State(销毁条件变量)

### Syntax

```c
int pthread_cond_destroy(pthread_cond_t *cv);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `cv` is invalid
