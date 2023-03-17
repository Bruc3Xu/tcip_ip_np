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

```c



```


### semaphore

### 线程销毁

