## IPV4(Internet Protocol Version 4)

4字节 = 网络地址+主机地址

A类地址：1字节网络地址+3字节主机地址，A类地址网络号范围:`0.0.0.0 ~ 126.0.0.0`，`10.0.0.0 ~ 10.255.255.255`是私有地址，`127.x.x.x`是测试用地址。

B类地址：2字节网络地址+2字节主机地址，B类IP地址范围`128.0.0.0 ~ 191.255.255.255`。

C类地址：3字节网络地址+1字节主机地址，C类IP地址范围`192.0.0.0 ~ 223.255.255.255`。

网络地址区分主机，端口号区分应用程序。16位端口号，0-65535，其中0-1023是约定的端口号，分配给固定的程序。TCP和UDP套接字可以重复使用一个端口号。


## IPV6(Internet Protocol Version 6)
16字节地址族


## 套接字结构体
确定一个套接字：地址族+IP地址+端口。

```c
struct sockaddr_in{
    sa_familly_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8]; //not used，为了与sockaddr保持一致插入的成员，填充0
};
struct in_addr{
    in_addr_t s_addr; //32位IPV4地址，uint32_t
};

struct sockaddr{
    sa_family_t sin_family;
    char sa_data[14]; // 地址+端口号，其他填充0
}

```
以上的结构体在POSIX（Portable Operating System Interface）标准中可以找到。

## 网络字节序和转换
- 大端序：高位字节存放到低位地址。
- 小端序：低位字节存放到低位地址。

```
0x12345678


大端

memory order
->
0x12  0x34  0x56 0x78

小端

memory order
->
0x78 0x56 0x34 0x12
```
约定网络传输使用大端字节序，主机一般是小端字节序。

转换函数
```c
#include <arpa/inet.h>
unsigned short htons(unsigned short); // short 2字节，用于端口号转换
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long); // long 4字节，用于IP地址转换
unsigned long ntohl(unsigned long);
```
上述代码不适用点分十进制表示法的IP地址，使用下面的函数转换。

```c
#include <arpa/inet.h>
in_addr_t inet_addr(const char* string);
// succ = unsigned long, error = INADDR_NONE

int inet_aton(const char* string, struct in_addr* addr);
// succ = 1, error = 0
// inet_aton("10.0.11.11", &sock_addr.s_addr);


char* inet_ntoa(struct in_addr addr);
// error = -1
// 结果存储到静态内存，重复调用会重写


// 适用于IPV6的地址转换
inet_ntop(int af, const void *__restrict cp, char *__restrict buf, socklen_t len)
inet_pton(int af, const char *restrict cp, void *restrict buf)

```

地址初始化使用bzero,相较于memset,少一个参数。

INADDR_ANY在不知道本机IP地址的情况下，使用它来代表本机所有接口的IP地址。








