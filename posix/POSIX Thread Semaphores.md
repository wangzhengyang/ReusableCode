# POSIX Thread Semaphores

[toc]

## Initialize a Semaphore(初始化信号量)

### Syntax

```c
int sem_init(sem_t *sem, int pshared, unsigned int value);
```

If the value of `pshared` is zero, then the semaphore cannot be shared between processes.If the value of `pshared` is nonzero, then the semaphore can be shared between processes.

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value argument exceeds `SEM_VALUE_MAX`
3. `ENOSPC` means a resource that is required to initialize the semaphore has been exhausted.The limit on semaphores `SEM_NSEMS_MAX` has been reached
4. `EPERM` means the process lacks the appropriate privileges to initialize the semaphore

## Incrementing a Semaphore(发送信号量)

### Syntax

```c
int sem_pos(sem_t *sem);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the `sem` points to an illegal address

## Blocking on a Semaphore Count(堵塞信号量)

### Syntax

```c
int sem_wait(sem_t *sem);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `sem` points to an illegal address
3. `EINTR` means a signal interrupted this function

## Decrementing a Semaphore Count(减少信号量计数)

### Syntax

```c
int sem_trywait(sem_t *sem);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `sem` points to an illegal address
3. `EINTR` means a signal interrupted this function
4. `EAGAIN` means the semaphore was already locked, so the semaphore cannot be immediately locked by the `sem_trywait()` operation

## Destroying the Semaphore State(销毁信号量)

### Syntax

```c
int sem_destroy(sem_t *sem);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means `sem` points to an illegal address