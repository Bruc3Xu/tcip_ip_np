## epoll
select的缺点：
- 轮询fds,效率低
- 每次调用之前需要向内核传递监视的fd
- 最大连接数量限制
优点：
- 兼容性好

epoll的优点：
- 无需轮询，直接返回就绪的fd
- 不用向内核传递需要监视的fd
- 没有最大连接数量限制，可以达到系统最大文件描述符数量

共性：
- 同时监听多个文件描述符
- timeout超时（精度不同）
- 返回文件描述符的数量
- 都使用某种结构体来告诉内核监听哪些事件，并使用结构体类型参数来获取内核的结果。



epoll()生成epoll实例，是内核中的一个数据结构，包含两个lists。
1. interest list（epoll set）：注册监控的文件描述符
2. ready list：interest list中文件描述符的引用的集合，I/O就绪。

```c
#include <sys/epoll.h>

int epoll_create(int size);
// 生成一个epoll实例，返回一个文件描述符指向这个实例
// size是向内核建议的文件描述符值
// succ = epollfd, err= -1

int epoll_ctl (int epfd, int op, int fd,
		      struct epoll_event *event);
// 注册文件描述符，将其加入到interst lsit
// op：添加、删除、更改操作
// EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD
// event：监视对象的事件类型

typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event
{
  uint32_t events;	/* Epoll events */
  epoll_data_t data;	/* User data variable */
} __EPOLL_PACKED;

// events：EPOLLIN, EPOLLOUT, EPOLLPRI(OOB), EPOLLRDHUP, EPOLLERR, EPOLLET, EPOLLONESHOT

int epoll_wait (int epfd, struct epoll_event *events,
		       int maxevents, int timeout);
// maxevents：events中可以保存的最大事件数
// timeout：超时毫秒，-1阻塞等待
```

## level trigger and edge trigger
水平触发和边缘触发
- 水平触发：只要输入缓冲中有数据就会一直通知该事件。
- 边缘触发：输入缓冲收到数据只注册一次该事件，即使输入缓冲中还有数据，也不会再次注册。

要使用边缘触发，就要一次性把输入缓冲中的数据读取出来，使用`errno=EAGAIN`来判断是否读完。

此外，边缘触发方式下，以阻塞方式工作的read和write可能因此服务器长时间的停顿，因而必须采用非阻塞读写。

> 边缘触发可以分离接收数据和处理数据的时间点。

