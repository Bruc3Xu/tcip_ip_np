# I/O复用

## 基于I/O复用的服务器端
多进程服务器端的缺点
- 每次新的客户端连接就会创建新的进程（创建和销毁进程的开销很大，进程池可以解决此问题）。
- 进程之前的数据交换复杂。
- 受限于进程数，只能处理少量的客户端连接。

I/O复用不是文件描述符的复用，而是进程或者线程的复用，能够监听多个文件描述符。使用场景如下：
- 处理多个socket
-  同时处理用户输入和网络连接。
- 同时处理listen和连接socket。（使用最多）
- 同时处理tcp和udp。
- 同时监听多个端口或者多种服务。
IO复用是同步阻塞模型，调用selelct，poll，epoll本身是阻塞的，但recv，write是非阻塞的。

## select系统调用
select函数调用方法和顺序
1. 设置文件描述符，指定监视范围，设置超时。
2. 调用select函数。
3. 查看调用结果。

### 准备
fdset是一个整形数组，每一位标记一个文件描述符（1表示对应的文件描述符是监视对象）。使用下列函数来操作这个数组，
```c
FD_ZERO(fdset* fdset); // 所有位置为0
FD_SET(int fd, fdset* fdset);  //将fd对应的位置为1
FD_CLR(int fd, fdset* fdset);  //将fd对应的位置为0
int FD_ISSET( int fd, fdset* fdset); //查看fd是否在fdset中
```

```c
#include <sys/select.h>
int select(int maxfd, fdset* readfds, fdset* writefds, fdset* exceptfds, struct timeval* timeout);
// 返回就绪的fd数量, error = -1, timeout = 0
// maxfd：监视对象（包括三个set）文件描述符数量，最大fd+1
// readfds：所有关注“是否有待读取数据”的fd注册到fdset,传递其地址
// writefds：所有关注“是否可传输无阻塞数据”的fd注册到fdset,传递其地址
// exceptfds：所有关注“是否发生异常”的fd注册到fdset,传递其地址
// timeout：超时信息

struct timeval{
    long tv_sec; // seconds
    long tv_usec; // microseconds
};

```
select的调用会阻塞到**有文件描述符可以进行IO操作**或被**信号打断**或者**超时**才会返回（即3种事件：IO事件、信号事件、定时事件）。
当读、写、异常（带外数据）就绪时，内核会修改传入内核的3个文件描述符集合。用户程序需要轮询这些集合，从数据内部属性的改变来判断是否可读写。因此，每次调用都要重新传入set（清零后）。