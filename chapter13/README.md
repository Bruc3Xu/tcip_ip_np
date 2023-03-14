# 多种I/O函数

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
