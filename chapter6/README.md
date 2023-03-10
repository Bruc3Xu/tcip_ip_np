与TCP不同，UDP不会进行流控制。UDP的最大作用是根据端口号将传到主机的数据包传递给最终的UDP套接字。

IO函数
```c
ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len);

ssize_t sendto (int __fd, const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len);
```
因为是无状态的连接，因此每次发送都要指定目标地址，不像TCP可以通过socket找到对应的对端地址。

recvfrom可以从socket得到发送方地址，sendto指定目标地址。

sendto发送时如果sock没有分配地址信息，则会自动分配（TCP客户端connect函数自动完成）。

## UDP套接字的数据边界
“发送一次，接收一次”，不能像TCP多次发送，一次接收。

UDP传输的数据包，本身可以成为一个完整的数据，称为数据报。

## 已连接和未连接UDP套接字
通过sendto发送数据报的过程：
1. 向UDP套接字注册目标IP和端口
2. 传输数据
3. 删除UDP套接字中注册的目标地址信息
通过上述方式，可以使用一个UDP套接字向不同目标传输数据。已经注册目标地址信息的套接字称为连接套接字，反之称为未连接套接字。
如果要和同一目标地址长时间通信，取消步骤3则会提升效率，即使用已连接套接字直接传输数据。

调用`connect`向UDP套接字注册目标地址信息，之后就可以使用read/write函数。
