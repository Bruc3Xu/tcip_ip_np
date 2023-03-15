# 多种I/O函数

## send和recv

```c
#include <sys/socket.h>

ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);
// succ = send bytes, err = -1


ssize_t recv(int sockfd, void* buf, size_t nbytes, int flags);
// succ = recv bytes, err = -1, EOF = 0

// flags
// MSG_OOB：传输带外数据
// MSG_PEEK：验证输入缓冲中是否存在接收的数据
// MSG_DONTROUTE：不参照ROUTE,在本地网络中寻找目的地
// MSG_DONTWAIT：非阻塞I/O调用
// MSG_WAITALL：防止函数返回，直至收到全部请求的字节数

```
### MSG_OOB
发送带外数据，拥有更高的优先级。UDP没有实现带外数据，TCP没有真正的带外数据，但是有紧急模式。TCP读取带外数据，每次读一个字节。

```c
send(sock, "890", strlen("890"), MSG_OOB);

/*
输出缓冲
-------------------------
偏移量0 1 2  偏移量3
   8    9 0  UrgentPointer
--------------------------

TCP数据包的结构
-------------------------------
TCP头                  |  数据
URG=1,UrgentPointer=3  | 8 9 0
-------------------------------

URG = 1表示是有紧急消息的数据包
UrgentPointer=3表示紧急指针位于偏移量3的位置
/*

```
紧急指针指向的偏移量之前的部分就是紧急消息。除了紧急指针前面的一个字节外，数据接收方通过调用常用输入函数读取剩余部分。紧急消息的意义在于督促消息处理，而不是紧急传输形式受限的消息。

### 检查输入缓冲
同时设置MSG_PEEK和MSG_DONTWAIT，以验证输入缓冲中是否存在接收的数据。
设置MSG_PEEK选项的recv，即使读取了输入缓冲中的数据也不会删除，常与MSG_DONTWAIT同时设置，调用以非阻塞方式验证待读数据是否存在。

```c
int str_len = recv(client_sock, msg, BUF_SIZE, MSG_PEEK | MSG_DONTWAIT);
```
## readv和writev
readv和writev对数据进行整合传输及发送，提高数据通信效率。
readv将消息由多个缓冲接收，writev可将多个缓冲中的数据合并发送，减少了I/O函数的调用次数。

```c
#include <sys/uio.h>

ssize_t writev(int fd, const struct iovec* iov, int iovcnt);
// succ = write bytes, err = -1
// iov：iovec数组地址
// invcnt：数组长度

struct iovec{
    void* iov_base; // 缓冲地址
    size_t iov_len; // 缓冲大小
};


ssize_t readv(int fd, const struct iovec* iov, int iovcnt);
```
使用readv和writev函数可以减少数据包的数量，尤其在关闭Nagle算法（TCPNODELAY=1）的情况下更有有利。
