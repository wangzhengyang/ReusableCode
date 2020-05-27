# POSIX Thread Read-Write Lock

[toc]

## Initializing a Read-Write Lock Attribute(初始化读写锁属性)

### Syntax

```c
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
```

### Return values

1. `0` means the call complete successfully
2. `ENOMEM` means insufficient memory exists to initialize the read-write attributes object

## Destroying a Read-Write Lock Attribute(销毁读写锁属性)

### Syntax

```c
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` is invalid

## Setting a Read-Write Lock Attribute(设置读写锁属性)

### Syntax

```c
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
```

### `pshared`

1. `PTHREAD_PROCESS_SHARED`
2. `PTHREAD_PROCESS_PRIVATE`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `pshared` is invalid

## Getting a Read-Write Lock Attribute(获取读写锁属性)

### Syntax

```c
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr, int *restrict pshared);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `pshared` is invalid

## Initializing a Read-Write Lock(初始化读写锁)

### Syntax

```c
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);

or

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `rwlock` is invalid

## Acquiring the Read Lock on Read-Write Lock(获取读锁)

### Syntax

```c
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `rwlock` is invalid

## Acquiring a Read Lock on a Read-Write Lock Before a Specified Absolute Time

### Syntax

```c
int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abs_timeout);
```

### Return values

1. `0` means the call complete successfully
2. `ETIMEDOUT` means the lock could not be acquired before the specified timeout expired
3. `EAGAIN` means the read lock could not be acquired because the maximum number of read locks for lock would be exceeded
4. `EDEADLK` means the calling thread already holds the rwlock
5. `EINVAL` means the value specified by `rwlock` does not refer to an initialized read-write lock object, or the timeout nanosecond value is less than zero or greater than or equal to 1000 million

## Acquiring a Non-Blocking Read Lock on a Read-Write Lock

### Syntax

```c
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means the read-write lock could not be acquired for reading because a writer holds the lock or was blocked on it

## Acquiring the Write Lock on a Read-Write Lock(获取写锁)

### Syntax

```c
int pthread_wrlock(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `rwlock` is invalid

## Acquiring a Non-blocking Write Lock on a Read-Write Lock

### Syntax

```c
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means the read-write lock could not be acquired for writing because the read-write lock is already locked for reading or writing

## Acquiring a Write Lock on a Read-Write Lock Before a Specified Absolute Time

### Syntax

```c
int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abs_timeout);
```

### Return values

1. `0` means the call complete successfully
2. `ETIMEDOUT` means the lock could not be acquired before the specified timeout expired
3. `EDEADLK` means the calling thread already holds the `rwlock`
4. `EINVAL` means the value specified by `rwlock` does not refer to an initialized read-write lock object, or the timeout nanosecond value is less than zero or greater than or equal to 1000 million

## Unlocking a Read-Write Lock(释放读写锁)

### Syntax

```c
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. an error nubmer is returned to indicate the error

## Destroying a Read-Write Lock(销毁读写锁)

### Syntax

```c
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `attr` or `rwlock` is invalid

