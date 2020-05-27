# POSIX Thread Spin Lock

[toc]

主要用于多处理器的内存数据共享，当一个线程请求自旋锁，但是该自旋锁已经被其他线程持有时，这个线程会一直不断地去测试自旋锁是否可用。当获取到自旋锁时，线程应该只持有很短的时间，因为自旋会浪费处理器周期。调用者在让线程睡眠之前应该释放掉自旋锁以便让其他线程获得锁



## Initializing a Spin Lock(初始化自旋锁)

### Syntax

```c
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
```

`PTHREAD_PROCESS_SHARED`允许任意可以访问该自旋锁内存空间的线程获取自旋锁

`PTHREAD_PROCESS_PRIVATE`进程内部的线程可以访问自旋锁

### `pshared`

1. `PTHREAD_PROCESS_SHARED`
2. `PTHREAD_PROCESS_PRIVATE`

### Return values

1. `0` means the call complete successfully
2. `EAGAIN` means the system lacks the neccessary resources to initialize another spin lock
3. `EBUSY` means the system has detected an attempt to initialize or destroy a spin lock while the lock is in use
4. `EINVAL` means the value specified by `lock` is invalid

## Acquiring a Spin Lock(获取自旋锁)

### Syntax

```c
int pthread_spin_lock(pthread_spinlock_t *lock);
```

### Return values

1. `0` means the call complete successfully
2. `EDEADLK` means the current thread already owns the spin lock
3. `EINVAL` means the value specified by `lock` does not refer to an initialized spin lock object

## Acquiring a Non-Blocking Spink Lock(无阻塞获取自旋锁)

### Syntax

```c
int pthread_spin_trylock(pthread_spinlock_t *lock);
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means a thread currently owns the spin lock
3. `EINVAL` means the value specified by `lock` does not refer to an initialized spin lock object

## Unlocking a Spin Lock(接触自旋锁)

### Syntax

```c
int pthread_spin_unlock(pthread_spinklock_t *lock);
```

### Return values

1. `0` means the call complete successfully
2. `EPERM` means the calling thread does not hold the lock
3. `EINVAL` means the value specified by `lock` does not refer to an initialized spin lock object

## Destroying a Spin Lock(销毁自旋锁)

### Syntax

```c
int pthread_spin_destroy(pthread_spinlock_t *lock);
```

### Return values

1. `0` means the call complete successfully
2. `EBUSY` means the system has detected an attempt to initialize or destroy a spin lock while the lock is in use
3. `EINVAL` means the value specified by `lock` is invalid