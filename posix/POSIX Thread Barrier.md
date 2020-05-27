# POSIX Thread Barrier

[toc]

## Initializing a Syncchronization Barrier

### Syntax

```c
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *restrict attr, unsigned count);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `count` is equal to 0, or the value specified by `attr` is invalid 
3. `EAGAIN` means the system lacks the necessary resources to initialize another barrier
4. `ENOMEM` means insufficient memory exists to initialize the barrier
5. `EBUSY` means there was an attempt to destroy a barrier while it is in use

## Waiting for Threads to Synchronize at a Barrier

### Syntax

```c
int pthread_barrier_wait(pthread_barrier_t *barrier);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means the value specified by `barrier` does not refer to an initialized barrier object

## Destroying a Synchronization Barrier

### Syntax

```c
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means indicates that the value of `barrier` was not valid
3. `EBUSY` means an attempt was made to destroy a barrier while it is in use

## Initializing a Barrier Attributes Object

### Syntax

```c
int pthread_barrierattr_init(pthread_barrierattr_t *attr);
```

### Return values

1. `0` means the call complete successfully
2. `ENOMEM` means insufficient memory exists to initialize the barrier attributes object

## Setting a Barrier Process-Shared Attribute

### Syntax

```c
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);
```

### `pshared`

1. `PTHREAD_PROCESS_PRIVATE`
2. `PTHREAD_PROCESS_SHARED`

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means indicates that the value of `attr` was not valid, or the new value specified for the `pshared` is not valid

## Getting a Barrier Process-Shared Attribute

### Syntax

```c
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict attr, int *restrict pshared);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means indicates that the value of `attr` was not valid

## Destroying a Barrier Attributes Object

### Syntax

```c
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
```

### Return values

1. `0` means the call complete successfully
2. `EINVAL` means indicates that the value of `attr` was not valid 