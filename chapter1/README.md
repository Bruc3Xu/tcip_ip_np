socket: socket就是操作系统提供给用户操作网络协议栈的接口，用户可以通过socket来操作网络协议栈的行为，进而达到网络通信的目的。

socket：Linux中本质是一个文件描述符，在“一切皆文件”的思想下，和一般打开文件得到的文件描述符没什么不同。

```c++
#include <sys/socket.h>

int scoket(int domain, int type, int protocol);
// succ = fd, err = -1
// domain: protocol family, PF_INET, PF_INET6, PF_LOCAL
// type: transfer type, TCP(SOCK_STREAM), UDP(SOCK_DGRAM)
// protocol: default 0, determined by the domain and type arguments

// bind to a specific addr, usually used by server to expose a fixed port for clients
int bind(int sockfd, const struct sockaddrr* __addr, socklen_t __addr_len);
// succ = 0, err = -1

int listen(int sockfd, int backlog);

int accept (int sockfd, struct sockaddrr* __addr,
		   socklen_t * __addr_len);
// succ = fd, err = -1

int connect (int sockfd, const struct sockaddrr* __addr, socklen_t __len);
```