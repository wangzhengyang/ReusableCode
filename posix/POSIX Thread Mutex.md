# POSIX Thread Mutex

[toc]

## Initializing a Mutex Attribute Object(初始化互斥量属性)

### Syntax

```c
int pthread_mutexattr_init(pthread_mutexattr_t *mattr);
```

### Default Attribute Values for `mattr`

| Attribute   | Value                      | Result                                                       |
| ----------- | -------------------------- | ------------------------------------------------------------ |
| pshared     | `PTHREAD_PROCESS_PRIVATE`  | The initialized mutex can be used within a process.Only those threads created by the same process can operate on the mutex(共享属性) |
| type        | `PTHREAD_MUTEX_DEFAULT`    | The Solaris Pthreads implementation maps `PTHREAD_MUTEX_DEFAULT` to `PTHREAD_MUTEX_NORMAL`, which does not detect deadlock |
| protocol    | `PTHREAD_PRIO_NONE`        | Thread priority and scheduling are not affected by the priority of the mutex owned by the thread |
| prioceiling | -                          | The prioceiling value is drawn from the existing priority range for the `SCHED_FIFO` policy, as returned by the `sched_get_priority_min()` and `sched_get_priority_max()` fucntions.This priority range is determined by the Solaris version on which the mutex is created |
| robustness  | `PTHREAD_MUTEX_STALLED_NP` | When the owner of a mutex dies,all future calls to `pthread_mutex_lock()` for this mutex will be blocked from progress |

### Return values

1. `0` means the call complete successfully
2. `ENOMEM` means insufficient memory exists to initialize the mutex attribute object

## Destroying a Mutex Attribute Object(销毁互斥量属性)

### Syntax

```c
int pthread_mutexattr_destroy(pthread_mutexattr_t *mattr);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `mattr` is invalid

## Setting the Scope of  a Mutex(设置互斥量共享属性)

### Syntax

```c
int pthread_mutexattr_setpshared(pthread_mutexattr_t *restrict mattr, int *restrict pshared);
```

`PTHREAD_PROCESS_SHARED`进程间共享互斥量

`PTHREAD_PROCESS_PRIVATE`进程内共享互斥量

### `pshared`

1. `PTHREAD_PROCESS_PRIVATE`
2. `PTHREAD_PROCESS_SHARED`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `mattr` is invalid

## Getting the Scope of a Mutex(获取互斥量共享属性)

### Syntax

```c
int pthread_mutexattr_getpshared(pthread_mutexattr_t *restrict mattr, int *restrict pshared);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `mattr` is invalid

## Setting the Mutex Type Attribute(设置互斥量类型属性)

### Syntax

```c
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```

`PTHREAD_MUTEX_NORMAL`不会侦测死锁。一个线程试图去锁住一个上锁的互斥量会引发死锁；试图去解锁一个其他线程的互斥量结果将导致无法定义的行为；试图去解锁一个已经解锁的互斥量也将导致无定义行为

`PTHREAD_MUTEX_ERRORCHECK`提供错误监测，一个线程试图去锁住一个已经上锁的互斥量会引发错误；试图去解锁一个其他线程已经上锁的互斥量会引发错误；线程试图去解锁一个已经解锁的互斥量也会引发错误

`PTHREAD_MUTEX_RECURSIVE`一个线程试图去锁住一个已经上锁的互斥量会成功。不会发生死锁问题。但是上锁的次数必须与解锁的次数相同，这样其他线程才可以获得互斥锁。如果一个线程试图去解锁一个其他线程已经上锁的互斥量会引发错误。线程试图去解锁一个已经解锁的互斥量会引发错误

`PTHREAD_MUTEX_DEFAULT`映射到其他互斥量类型

### `type`

1. `PTHREAD_MUTEX_NORMAL`
2. `PTHREAD_MUTEX_ERRORCHECK`
3. `PTHREAD_MUTEX_RECURSIVE`
4. `PTHREAD_MUTEX_DEFAULT`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value `type` or `attr` is invalid

## Getting the Mutex Type Attribute(获取互斥量类型属性)

### Syntax

```c
int pthread_mutexattr_gettype(pthread_mutexattr_t *restrict attr, int *restrict type);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `type` is invalid

## Setting the Mutex Attribute's Protocol(设置互斥量属性协议)

### Syntax

```c
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
```

`PTHREAD_PRIO_NONE`线程的优先级以及调度策略不会被互斥量拥有者影响

`PTHREAD_PRIO_INHERIT`避免优先级反转，协议影响线程的优先级以及调度。当高优先级因为一个或多个互斥量锁住导致堵塞，其中互斥量由`thrd1`设置初始化属性为`PTHREAD_PRIO_INHERIT`,则`thrd1`的优先级提升到`thrd1`优先级与被堵塞线程的优先级的最大值；如果`thrd1`的堵塞，因为`thrd3`的互斥量，则`thrd3`的优先级也会被提高

`PTHREAD_PRIO_PROTECT`协议会影响线程的优先级以及调度。当`thrd2`拥有一个或多个互斥量，并且互斥量的属性设置为`PTHREAD_PRIO_PROTECT`,则`thrd2`的优先级会提高到最高优先级阈值，让`thrd2`优先完成执行，其他持有该互斥量的高优先级线程会堵塞

`PTHREAD_PRIO_INHERIT``PTHREAD_PRIO_PROTECT`只用于实时调度系统`SCHED_FIFO`以及`SCHED_RR`

### `protocol`

1. `PTHREAD_PRIO_NONE`
2. `PTHREAD_PRIO_INHERIT`
3. `PTHREAD_PRIO_PROTECT`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `protocol` is not valid
3. `EPERM` means the caller does not have the privilege to perform the operation

## Getting the Mutex Attribute's Protocol(获取线程属性协议)

### Syntax

```c
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict attr, int *restrict protocol);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` is `NULL`, or the value specified by `attr` or `protocol` is invalid
3. `EPERM` means the caller does not have the privilege to perform the operation

## Setting the Mutex Attribute's Priority Ceiling(设置互斥量属性优先级最大值)

### Syntax

```c
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
```

未避免优先级反转，将`prioceiling`值设置为使用该互斥量的所有线程的最高优先级之上，且要小于等于`SCHED_FIFO`定义的最大优先级

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` is `NULL` or invalid or `prioceiling` is invalid
3. `EPERM` means the caller does not have the privilege to perform the operation

## Getting the Mutex Attribute's Priority Ceiling(获取互斥量属性优先级最大值)

### Syntax

```c
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *restrict attr, int *restrict prioceiling);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` is `NULL`
3. `EPERM` means the caller does not have the privilege to perform the operation

## Setting the Mutex's Priority Ceiling(设置互斥量优先级最大值)

### Syntax

```c
int pthread_mutex_setprioceiling(pthread_mutex_t *restrict mutex, int prioceiling, int *restrict old_ceiling);
```

可以在程序运行时动态修改该运行值

如果互斥量没有上锁则锁住互斥量，否则等待直到成功锁住互斥量，然后改变优先级最大值最后释放互斥量

### Return values

1. `0` means the call complete successfully
2. `EINVAL` the priority requested by `prioceiling` is out of range or the mutex was not initialized with its `protocol` attribute having the value of `THREAD_PRIO_PROTECT`
3. `EPERM` means the caller does not have the privilege to perform the operation

## Getting the Mutex's Priority Ceiling(获取互斥量优先级最大值)

### Syntax

```c
int pthread_mutex_getprioceiling(const pthread_mutex_t *restrict mutex, int *restrict prioceiling);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `mutex` does not refer to a currently existing mutex
3. `EPERM` means the caller does not have the privilege to perform the operation

## Setting the Mutex's Robust Attribute(暂不使用)

### Syntax

```c
int pthread_mutexattr_setrobust_np(pthread_mutexattr_t *attr, int robustness);
```

### `robustness`

1. `PTHREAD_MUTEX_STALLED_NP`
2. `PTHREAD_MUTEX_ROBUST_NP`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `robustness` is invalid

## Getting the Mutex's Robust Attribute(暂不使用)

### Syntax

```c
int pthread_mutexattr_getrobust_np(const pthread_mutexattr_t *attr, int *robustness);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `robustness` is invalid

## Initializing a Mutex(初始化互斥量)

### Syntax

```c
int pthread_mutex_init(pthread_mutex_t *restrict mp, const pthread_mutexattr_t *restrict mattr);

pthread_mutex_t mp = PTHRAD_MUTEX_INITIALIZER;
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means the implementation has detected an attempt o reinitalize the object referenced by `mp`, a previously initialized but not yet destroyed mutex
3. `EINVAL` means the `mattr` attribute value is invalid.The mutex has not been modified
4. `EFAULT` means the address for the mutex pointed at by `mp` is invalid

## Making a Mutex Consistent(暂不使用)

### Syntax

```c
int pthread_mutex_consistent_np(pthread_mutex_t *mutex);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the current thread does not own the mutex or the mutex is not a `PTHREAD_MUTEX_ROBUST_NP` mutex having an inconsistent state

## Locking a Mutex(锁住互斥量)

### Syntax

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```

### Return values

1. `0` means the call complete successfully
2. `EAGAIN` means the mutex could not be acquired because the maximum number of recursive locks for mutex has been exceeded
3. `EDEADLK` means the current thread already owns the mutex

if set `PTHREAD_MUTEX_ROBUST_NP` attribute,the return  have following values:

1. `EOWNERDEAD`
2. `ENOTRECOVERABLE`
3. `ENOMEM`

## Unlocking a Mutex(释放互斥量)

### Syntax

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

### Return values

1. `0` means the call complete successfully
2. `EPERM` means the current thread does not own the mutex

## Locking a Mutex Without Blocking(不堵塞锁住互斥量)

### Syntax

```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means the mutex could not be acquired because the mutex pointed to by `mutex` was already locked
3. `EAGAIN` means the mutex could not be acquired because the maximum number of recursive locks for `mutex` has been exceeded

other but not always use:

	1. `EOWNERDEAD`
 	2. `ENOTRECOVERABLE`
 	3. `ENOMEM`

## Locking a Mutex Before a Specified Absolute Time(超时等待锁住互斥量)

### Syntax

```c
#include <time.h>
int pthread_muetx_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the mutex was created with the protocol attribute having the value `PTHERAD_PRIO_PROTECT` and the calling thread's priority is higher than the mutex's current priority ceiling, the value specified by `mutex` does not refer to an initialized mutex object, the process or thread would have blocked, and the `abs_timeout` parameter specified a nanoseconds field value less than 0 or greater than or equal to 1000 million
3. `ETIMEDOUT` means the mutex could not be locked before the specified timeout expired

## Locking a Mutex Within a Specified Time Interval(暂不使用)

### Syntax

```c
int pthread_mutex_reltimedlock_np(pthread_mutex_t *restrict mutex, const struct timespec *restrict rel_timeout);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the mutex was created with the protocol attribute having the value `PTHREAD_PRIO_PROTECT` and the calling thread's priority is higher than the mutex's current priority ceiling, the value specified by `mutex` does not refer to an initialized mutex object, the process or thread would have blocked, and the `abs_timeout` parameter specified a nanoseconds field value less than 0 or greater than or equal to 1000 million
3. `ETIMEDOUT` means the mutex could not be locked before the specified timeout expired

## Destroying a Mutex(销毁互斥量)

### Syntax

```c
int pthread_mutex_destroy(pthread_mutex_t *mp);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `mp` does not refer to an initialized mutex object