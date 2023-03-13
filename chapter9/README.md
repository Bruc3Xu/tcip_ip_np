套接字具有多种特性，可以查看和设置。

这些可选项可以根据协议层分为三层：SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP，个别选项只能进行查看，不能设置。

查看和设置的函数：
```c
#include <sys/socket.h>

int getsockopt(int fd, int level, int optname, void* optval, socklen_t* optlen);
//succ = 0, err = -1

int setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen);
//succ = 0, err = -1
```
## SO_TYPE
通过SO_TYPE查看socket类型

## SO_RCVBUF和SO_SNDBUF
通过SO_RCVBUF和SO_SNDBUF来查看和修改输入和输出的缓冲区大小。
```
tcip_ip_np/chapter9$ ./get_buf
recv buffer size: 131072
send buffer size: 16384

xrc@ubuntu:~/dev/repo/tcip_ip_np/chapter9$ ./set_buf
recv buffer size: 6144
send buffer size: 6144
```
缓冲区大小最小为6144。

## SO_REUSEADDR
通过SO_REUSEADDR将Time-wait状态下（等待3分钟左右清除）的套接字端口号分配给新的套接字。无论是服务器端还是客户端，
先断开连接（即主动断开）的套接字经过四次挥手后都会处于Time-wait状态，相应的端口还在使用中，只不过客户端的端口是动态分配的，不会重复导致绑定错误。

为什么会有Time-wait这一过程：如果没有Time-wait这一过程，主动断开连接的主机最后向对端发送的ACK消息后就直接清除socket,这个消息如果丢失，则对端主机则永远无法收到。
反之，可以重新发送。

## TCP_NODELAY
TCP套接字默认使用Nagle算法来交换数据，因此最大限度地进行缓冲，直到收到ACK。Nagle算法应用于TCP层，只有收到前一数据的ACK消息时，Nagle算法才会发送下一数据。

一般情况下，不使用Nagle算法可以提高传输速度（例如传输大文件数据），但是无条件放弃Nagle算法则会导致网络流量过多，反而影响传输。

使用TCP套接字选项TCP_NODELAY可以禁用Nagle算法。
```c
int optval = 1;
socklen_t optlen = sizeof(optval);
int state = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&optval, optlen);
```