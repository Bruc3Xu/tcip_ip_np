使用shutdown来关闭一个连接，可以关闭单独的输入或者输出。
```c
int shutdown(int sockfd, int how);
/*
SHUT_RD
SHUT_WR
SHUT_RDWR
*/

```

此外，使用close只是使socket的引用数减一，并不会直接关闭socket，等到引用计数为0才会关闭。
而shutdown直接发送fin来关闭TCP连接。

一个常见的问题：子进程继承父进程的socket,使用close关闭，导致父进程的socket处于CLOSE_WAIT状态。
正确的做法是：先使用`shutdown(fd, SHUT_RDWR); close(fd);`
