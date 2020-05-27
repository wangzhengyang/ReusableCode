# POSIX Thread

[toc]

## Creating a Default Thread(创建默认线程)

### Syntax

```c
int pthread_create(pthread_t *restrict tid, const pthread_attr_t *restrict tattr, void* (*start_routine)(void*), void *restrict arg);
```

### Attribute(线程属性)

when `tattr` is not specified,  means `NULL`, the default thread is created with the following attributes:

1. Process scope
2. Nondetached 
3. A default stack and stack size
4. A priority of zero

### Return values

1. `0` means the call completes successfully
2. `EAGAIN` means a system limit is exceeded, such as too many threads have been created(线程个数达到上限)
3. `EINVAL` means the value of `attr` is invalid(线程属性无效)
4. `EPERM` means the caller does not have appropriate permission to set the required  scheduling parameters or scheduling policy(调用者没有权限设置调度参数以及类型)

## Waiting for Thread Termination(等待线程终止)

### Syntax

```c
int pthread_join(pthread_t tid, void **status);
```

线程必须属于当前进行，并且处于未分离状态

多线程等待一个线程终止，多线程都会堵塞，并且只有一个线程返回成功，其余都将返回`ESRCH`错误字

在`pthread_join`后，才会释放掉线程的所有资源

在没有数据同步的情况下，默认线程创建应该属于分离态

### `Status`(线程返回状态)

when `status` is not `NULL`, `status` points to a location that is set to the exit status of the terminated thread  when function returns successfully

`status`为线程退出时通过`return`或者`exit`传递出来的返回值

### Return Values

1. `0` means the call completes successfully
2. `ESRCH` means no thread could be found corresponding to the given thread ID(未找到对应的线程ID)

if multiple threads wait for the same thread to terminate, all the threads wait until the target thread terminates.Then one waiting thread returns successfully.The other waiting threads failed with an error of `ESRCH`

3. `EDEADLK` means a deadlock would exist, such as a thread waits for itself or thread A waits for thread B and thread B waits for thread A(存在死锁)
4. `EINVAL` means the thread corresponding to the given thread ID is a detached thread(等待的线程处于分离状态)

## Detaching a Thread(线程设置为分离态)

### Syntax

```c
int pthread_detach(pthread_t tid);
```

分离态线程会在终止时，由应用程序自动回收资源

### Return values

1. `0` means the call completes successfully
2. `EINVAL` means `tid` is a detached thread(线程已经是分离状态)
3. `ESRCH` means `tid` is not valid, undetached thread in the current process(当前进行线程id无效或者处于未分离状态)

## Creating a Key for Thread-Specific Data(创建线程独有数据键值)

### Syntax

```c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
```

线程独有数据存在每个线程中，都通过绑定一个键值(全局变量)以便后来调用线程独有数据

键值创建后，每个线程都可以通过该键值绑定不同的数据类型

通过键值，一个线程可以获取自己线程独有的数据值

通过`destructor`函数可以在线程终止时，释放独有的数据

### Return values

1. `0` means the call completes successfully
2. `EAGAIN` the `key` name space is exhausted(键值名字空间已经使用光了)
3. `ENOMEM` Insufficient virtual memory is available in this processs to create a new key(没有足够的虚拟空间用于线程创建一个新的键值)

## Deleting the Thread-Specific Data Key(销毁键值)

### Syntax

```c
int pthread_key_delete(pthread_key_t key);
```

释放任何跟键值有关的内存数据(不包括跟键值绑定的数据类型)

在`pthread_key_delete`之前必须释放键值所绑定的数据区域

反复的使用`pthread_key_create()`以及`pthread_key_delete()`会导致内存空间消耗完

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the `key` value is invalid(键值无效)

## Setting Thread-Specific Data(绑定键值数据)

### Syntax

```c
int pthread_setspecific(pthread_key_t key, const void *value);
```

在键值绑定新值时，并不会释放掉原有绑定的数据类型，所以必须释放掉，否则会导致内存泄露

### Return values

1. `0` means the call complete successfully
2. `ENOMEM` means Insufficient virtual memory is available(没有足够的虚拟内存空间)
3. `EINVAL` means `key` is invalid(键值无效)

## Getting Thread-Specific Data(获取键值数据)

### Syntax

```
void *pthread_getspecific(pthread_key_t key);
```

## Getting the Thread Identifier(获取线程标识符)

### Syntax

```c
pthread_t pthread_self(void);
```

### Return values

return the thread identifier of the calling thread

## Comparing Thread IDs(比较线程ID)

### Syntax

```c
int pthread_equal(pthread_t tid1, pthread_t tid2);
```

### Return values

nonzero when `tid1` and `tid2` are equal, otherwise, `0` is returned

## Calling an Initialzation Routine for a Thread(线程初始化函数)

### Syntax

```c
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *onec_control, void (*init_routine)(void));
```

在第一次`pthread_once`调用时初始化例程，接下来调用`pthread_once`时不会调用初始化例程，也就是说只进行一次初始化

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `conce_control` or `init_routine` is `NULL`(值或者初始化函数为空)

## Yielding Thread Execution(线程挂起)

### Syntax

```c
#include <sched.h>
int sched_yield(void);
```

引起当前线程挂起以便让其他优先级更高或者相等的线程运行，如果没有，则当前线程继续运行

### Return values

1. `0` means the call complete successfully
2. `-1` means `errno` is set to indicate the error condition

## Setting the Thread Policy and Scheduling Parameters(设置线程调度策略以及参数)

### Syntax

```c
int pthread_setschedparam(pthread_t tid, int policy, const struct sched_param *param);

policy:
1. SCHED_FIFO
2. SCHED_RR
3. SCHED_OTHER
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of the attribute being set is not valid(属性无效)
3. `EPERM` means the caller does not have the appropriate permission to set either the scheduling parameters or the scheduling policy of the specific thread(没有权限)
4. `ESRCH` means the value specified by `tid` does not refer to an existing thread(线程不存在)

## Getting the Thread Policy and Scheduling Parameters(获取线程调度策略以及参数)

### Syntax

```c
int pthread_getschedparam(pthread_t tid, int *restrict policy, struct sched_param *restrict param);
```

### Return values

1. `0` means the call complete successfully
2. `ESRCH` means the value specified by `tid` does not refer to an existing thread(线程不存在)

## Setting the Thread Priority(设置线程优先级)

### Syntax

```c
int pthread_setschedprio(pthread_t tid, int prio);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `prio` is invalid for the scheduling policy of the specified thread(对特定调度策略优先级值无效)
3. `ENOTSUP` means an attempt was made to set the priority to an unsupported value(优先级值无效)
4. `EPERM` means the caller does not have the appropriate permission to set the scheduling priority of the specified thread(无权限)
5. `ESRCH` means the value specified by `tid` does not refer to an existing thread(线程无效)

## Sending a Signal to a Thread(给线程发送信号)

### Syntax

```c
int pthread_kill(pthread_t tid, int sig);
```

`sig`设置为0，则进行错误检测，但不会发送任何信号，可以根据特性检查线程ID的有效性(即该线程有没有挂掉)

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `sig` is not a valid signal number
3. `ESRCH` means `tid` cannot be found in the current process

## Accessing the Signal Mask of the Calling Thread(更改线程信号屏蔽位)

### Syntax

```c
#include <signal.h>
int pthread_sigmask(int how, const sigset_t *new, sigset_t *old); 
```

### `how`

1. `SIG_BLOCK` add `new` to the current signal mask, where `new` indicates the set of signals to block

添加信号屏蔽字到当前的信号屏蔽字

2. `SIG_UNBLOCK` delete `new` from the current signal mask, where `new` indicates the set of signals to unblock

从当前的信号屏蔽字中删除信号屏蔽字

3. `SIG_SETMASK` Replace the current signal mask with `new`, where `new` indicates the new signal mask

替换新的信号屏蔽字

获取当前线程的信号屏蔽字，可以设置`new`为`NULL`，从`old`中读取

### Return values

1. `0` means the call complete successfully
2. `EINVAL` the value of `how` is not defined and `old` is `NULL`

## Froking Safely(暂时不使用)

### Syntax

```c
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
```

### Retrun values

1. `0` means the call complete successfully
2. `ENOMEM` means insufficient table space exists to record the fork handler address

## Terminating a Thread(线程自我终止)

### Syntax

```c
void pthread_exit(void *status);
```

线程终止自己的执行通过以下几种方式：

1. 通过从他第一次程序调用(线程的启动例程)返回
2. 通过调用`pthread_exit()`
3. 通过POSIX的终止函数`pthread_cancel()`

注意主线程`main`函数通过`return`或者`exit`都将会导致整个进程终止，所以会导致所有线程都退出，假如主线程`main`调用`pthread_exit()`,则主线程只会终止自己，其他线程继续执行，直到最后一个线程退出，进程才会终止

### Return values

the calling thread terminates with its exit status set to the contents of `status`

## Cancelling a Thread(终止其他线程)

### Syntax

```c
int pthread_cancel(pthread_t tid);
```

终止请求是否生效依靠目标线程的状态，通过`pthread_setcancelstate()`以及`pthread_setcanceltype()`来改变状态

### Return values

1. `0` means the call complete successfully
2. `ESRCH` means no thread could be found corresponding to that specified by the given thread ID

## Enable or Disabling Cancellation(使能线程是否可以被终止)

### Syntax

```c
int pthread_setcanclestate(int state, int *oldstate);
```

线程创建时默认可以被终止

### `state`

1. `PTHREAD_CANCEL_ENABLE`
2. `PTHREAD_CANCEL_DISABLE`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the `state` is not `PTHREAD_CANCEL_ENABLE` or `PTHREAD_CANCEL_DISABLE`

## Setting Cancellation Type(设置现场终止类型)

### Syntax

```c
int pthread_setcanceltype(int type, int *oldtype);
```

线程创建时默认`deferred`模式，该线程只能在一些点终止，`asynchronous`模式可以在运行时刻中的任意一个点被终止，不建议使用该模式

### `type`

1. `PTHREAD_CANCEL_DEFERRED`
2. `PTHREAD_CANCEL_ASYNCHRONOUS`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the `type` is not `PTHREAD_CANCEL_DEFERRED` or `PTHREAD_CANCEL_ASYNCHRONOUS`

## Creating a Cancellation Point(创建一个线程终止点)

### Syntax

```c
void pthread_testcancel(void);
```

需要在一些安全的点插入线程终止点

## Pushing a Handler Onto the Stack(设置清理函数)

### Syntax

```c
void pthread_cleanup_push(void (*routine)(void*), void *args);
```

## Pulling a Handler Off the Stack(提取清理函数)

### Syntax

```c
void pthread_cleanup_pop(int execute);
```

在线程退出时会默认调用

