# POSIX Thread Attributes

[toc]

## Initializing Attributes(线程属性初始化)

### Syntax

```c
int pthread_attr_init(pthread_attr_t *tattr);
```

线程属性初始化过后，可以在进程内使用

### Default Attribute Values for `tattr`

| Attribute    |          Value          | Result                                                       |
| ------------ | :---------------------: | ------------------------------------------------------------ |
| scope        |  PTHREAD_SCOPE_PROCESS  | New thread contends with other threads in the process(进程内资源竞争) |
| detachstate  | PTHREAD_CREATE_JOINABLE | Completion status and thread ID are preserved after the thread exits(非分离态) |
| stackaddr    |          NULL           | New thread has system-allocated stack address(栈地址)        |
| stacksize    |            0            | New thread has system-defined stack size(栈大小)             |
| priority     |            0            | New thread has priority 0(优先级)                            |
| inheritsched | PTHREAD_EXPLICIT_SCHED  | New thread does not inherit parent thread scheduling priority(优先级继承) |
| schedpolicy  |       SCHED_OTHER       | New thread uses the traditional Solaris time-sharing(TS)scheduling class(调度策略) |
| guardsize    |        PAGESIZE         | Stack overflow protection(栈溢出保护)                        |

### Return values

1. `0` means the call complete successfully
2. `ENOMEN` means not enough memory is allocated to initialize the thread attributes object

## Destroying Attributes(销毁线程属性)

### Syntax

```c
int pthread_attr_destroy(pthread_attr_t *tattr);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means that the `tattr` was not valid

## Setting Detach State(设置线程分离态)

### Syntax

```c
int pthread_attr_setdetachstate(pthread_attr_t *tattr, int detachstate);
```

当使用`PTHREAD_CREATE_DETACHED`创建线程时，在线程退出时会自动释放线程ID以及其他资源

当使用`PTHREAD_CREATE_JOINABLE`创建线程时，在线程退出时需要使用`pthread_join()`来回收资源

无论线程以何种状态创建，必须等待所有的线程都退出后才能让进程退出

建议使用分离态

### `detachstate`

1. `PTHREAD_CREATE_JOINABLE`
2. `PTHREAD_CREATE_DETACHED`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `detachstate` or `tattr` was not valid

## Getting the Detach State(获取线程分离态)

### Syntax

```c
int pthread_attr_getdetachstate(const pthread_attr_t *tattr, int *detachstate);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `detachstate` is `NULL` or `tattr` is invalid

## Setting the Stack Guard Size(设置栈保护值大小)

### Syntax

```c
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
```

如果出现栈溢出，则信号`SIGSEGV`会发送给该线程

### Return values

1. `0` means the call complete successfully
2. `EINVAL` the `attr` is invalid, `guardsize` is invalid or the `guardsize` contains an invalid value

## Getting the Stack Guard Size(获取栈保护值大小)

### Syntax

```c
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr, size_t *restrict guardsize);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` the `attr` is invalid, `guardsize` is invalid or the argument `guardsize` contains an invalid value

## Setting the Scope(设置线程竞争属性)

### Syntax

```c
int pthread_attr_setscope(pthread_attr_t *tattr, int scope);
```

`PTHREAD_SCOPE_SYSTEM`线程在系统内竞争

`PTHREAD_SCOPE_PROCESS`线程在进程内竞争

### `scope`

1. `PTHREAD_SCOPE_SYSTEM`
2. `PTHREAD_SCOPE_PROCESS`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means an attempt was made to set `tattr` to a value that is not valid

## Getting the Scope(获取线程竞争属性)

### Syntax

```c
int pthread_attr_getscope(pthread_attr_t *restrict tattr, int *restrict scope);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `scope` is `NULL` or `tattr` is invalid

## Setting the Thread Concurrency Level(暂不使用)

### Syntax

```c
int pthread_setconcurrency(int new_level);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `new_level` is negative
3. `EAGAIN` means the value specified by `new_level` would cause a system resources to be exceeded

## Getting the Thread Concurrency Level(暂不使用)

### Syntax

```c
int pthread_getconcurrency(void);
```

### Return values

`phtread_getconcurrency()` always returns the concurrency level set by a previous call to `pthread_setconcurrency()`. if `pthread_setconcurrency()` has never been called, `pthread_getconcurrency()` returns zero

## Setting the Scheduling Policy(设置调度策略)

### Syntax

```c
int pthread_attr_setschedpolicy(pthread_attr_t *tattr, int policy);
```

`SCHED_FIFO`基于先进先出的实时调度机制的线程，要求该进程有权限设置，如果不是被高优先级的线程抢占，则线程会一直运行直到自己挂起或者堵塞

`SCHED_RR`竞争属性设置为`PTHREAD_SCOPE_SYSTEM`的基于时间片轮转的线程并且其有效用户ID为0(root)，则该线程属于实时调度。除非被其他更高优先级抢占、自身挂起或堵塞，否则基于系统的时间片运行。竞争属性设置为`PTHREAD_SCOPE_PROCESS`的基于时间片轮转的线程是基于分时调度，此外，这些线程所属的进程不需要有效用户ID为0

### `policy`

1. `SCHED_FIFO`
2. `SCHED_RR`
3. `SCHED_OTHER`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means an attempt was made to set `tattr` to a value that is not valid
3. `ENOTSUP` means an attempt was made to set the attribute to an unsupported value

## Getting the Scheduling Policy(获取调度策略)

### Syntax

```c
int pthread_attr_getschedpolicy(pthread_attr_t *restrict tattr, int *restrict policy);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the parameter `policy` is `NULL` or `tattr` is invalid

## Setting the Inherited Scheduling Policy(设置可继承调度策略属性)

### Syntax

```c
int pthread_attr_setinheritsched(pthread_attr_t *tattr, int inheritsched);
```

### `inheritsched`

1. `PTHREAD_INHERIT_SCHED`
2. `PTHREAD_EXPLICIT_SCHED`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means an attempt was made to set `tattr` to a value that is not valid

## Getting the Inherited Scheduling Policy(获取可继承调度策略属性)

### Syntax

```c
int pthread_attr_getinheritsched(pthread_attr_t *restrict tattr, int *restrict inheritsched);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the parameter `inheritsched` is `NULL` or `tattr` is invalid 

## Setting the Scheduling Parameters(设置调度参数)

### Syntax

```c
int pthread_attr_setschedparam(pthread_attr_t *restrict tattr, const struct sched_param *restrict param);
```

`SCHED_OTHER`设置优先级无效

管理线程优先级的方法：1.在创建线程时设置优先级属性；2.可以改变自身的优先级然后在改变回来

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `param` is `NULL` or `tattr` is invalid

### Manage pthreads priority in either of two ways

1. set the priority attribute before creating a child thread
2. change the priority of the parent thread and then change the priority back

## Getting the Scheduling Parameters(获取调度参数)

### Syntax

```c
int pthread_attr_getschedparam(pthread_attr_t *restrict tattr, const struct sched_param *restrict param);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `param` is `NULL` or `tattr` is invalid

## 关于栈

线程栈通常都以页(pagesize)对齐，任何栈尺寸都将会转化为页大小的整数倍。一个没有写权限的页追加在栈的最后用于检测栈溢出，绝大多数的栈溢出都会发送信号`SIGSEGB`到线程。如果是用户指定的栈，则不会改变大小

32bit系统默认分配1M的栈大小，64bit系统默认分配2M的栈大小，没有交换空间

## Setting the Stack Size(设置栈大小)

### Syntax

```c
int pthread_attr_setstacksize(pthread_attr_t *tattr, size_t size);
```

`size` contains the number of bytes for the stack that the new thread uses. if `size` is zero, a default size is used. In most cases, a zero value works best

`PTHREAD_STACK_MIN` is the amount of stack space that is required to start a thread

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `size` is less than `PTHREAD_STACK_MIN`,or exceeds a system-imposed limit,or `tattr` is not valid

## Getting the Stack Size(获取栈大小)

### Syntax

```c
int pthread_attr_getstacksize(pthread_attr_t *restrict tattr, size_t *restrict size);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `tattr` or `size` is not valid

## Setting the Stack Address and Size(设置栈大小及地址)

### Syntax

```c
int pthread_attr_setstack(pthread_attr_t *tattr, void *stackaddr, size_t stacksize);
```

the `stackaddr` attribute defines the base (low address) of the thread's stack.The `stacksize` attribute specifies the size of the stack. if `stackaddr` is `NULL`, then  `pthread_create()` allocates a stack for the new thread with at least `statcksize` bytes

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `stackaddr` is incorrect or the value of `stacksize` is less than `PTHREAD_STACK_MIN`

## Getting the Stack Address and Size(获取栈大小及地址)

### Syntax

```c
int pthread_attr_getstack(pthread_attr_t *restrict tattr, void **restrict stackaddr, size_t *restrict stacksize);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value of `tattr` is incorrect