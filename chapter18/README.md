# 多线程服务器端的实现
多进程服务器端的缺点
- 创建进程的开销
- 进程通信的开销
- 上下文切换的开销（很大）

## 线程
线程作为轻量级的进程，在后两个方面具有很大的优势。

每个进程都具有自己的数据区、动态分配的堆区域和运行时的栈区域。

如果以获得多个代码执行流为目的，则只应该分离栈区域，即线程所做的。

```c
#include <pthread.h>
int pthread_create (pthread_t * thread_id,
			   const pthread_attr_t * thread_attr,
			   void *(*start_routine) (void *),
			   void * arg);
// succ = 0, err = EAGAIN, EINVAL, EPERM
void* start_routine(void* arg);


int pthread_join (pthread_t thread_id, void **thread_return);
// succ = 0, err = EDEADLK, EINVAL, ESRCH

```

大多数标准函数都是线程安全的函数，可以在临界区内安全访问，非线程安全的函数也有对应的替代，如`gethostbyname`对应`gethostbyname_r`。

使用`-D_REENTRANT`宏，使用线程安全版本的函数来替换原有的函数。

## 线程同步

### mutex
使用mutex来保护临界区的数据。
```c
int pthread_mutex_init (pthread_mutex_t *mutex,
			       const pthread_mutexattr_t *mutexattr);
int pthread_mutex_destroy (pthread_mutex_t *mutex);

int pthread_mutex_lock (pthread_mutex_t *mutex);

int pthread_mutex_unlock (pthread_mutex_t *mutex);
```
需要注意的是，需要考虑mutex加锁的范围，合理使用。

### semaphore

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
// pshared: 0对应多线程共享，sem应该对于所有线程是可见的（全局变量或者堆上的变量）
// pshared > 0：对应多进程共享，sem位于共享内存区域（子进程和父进程共享）。
// value：初始值


int sem_post(sem_t *sem);
// value值加一

int sem_wait(sem_t *sem);
// value值减一
// 在value为0时调用，会阻塞

```


### 线程销毁
相对于pthread_join，pthread_detach不会引起主线程阻塞。
```c
int pthread_detach(pthread_t thread);
```

## 多线程服务端
书中实现的是one thread one connection。