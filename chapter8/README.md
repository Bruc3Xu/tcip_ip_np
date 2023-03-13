# DNS(Domain Name system)
DNS域名系统是对IP地址和域名进行相互转换的系统，核心是DNS服务器。

DNS系统是分层的树状系统，请求DNS服务解析域名会逐级向上直到找到对应的IP地址。

域名IP地址转换函数
```c
#include <netdb.h>

struct hostent * gethostbyname(const char* hostname);

struct hostent{
    char* h_name; //official name
    char** h_aliases; // alias list
    int h_addrtype; // host address type
    int h_length; // address length
    char** h_addr_list; // address list, in_addr struct*
};

```

一些结果
```text
tcip_ip_np/chapter8$ ./a.out www.baidu.com

official name: www.a.shifen.com
alias name 1: www.baidu.com
addr type: IPV4
addr 1: 180.101.50.188
addr 2: 180.101.50.242

tcip_ip_np/chapter8$ ./a.out www.google.com

official name: www.google.com
addr type: IPV4
addr 1: 74.125.204.105
addr 2: 74.125.204.104
addr 3: 74.125.204.103
addr 4: 74.125.204.147
addr 5: 74.125.204.99
addr 6: 74.125.204.106

tcip_ip_np/chapter8$ ./a.out www.github.com

official name: www.github.com
addr type: IPV4
addr 1: 20.205.243.166
```

利用IP地址获取域名
```c
struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family);

// addr: in_addr struct*

```

```text
tcip_ip_np/chapter7$ ping google.com

PING google.com (142.251.8.101) 56(84) bytes of data.
64 bytes from tb-in-f101.1e100.net (142.251.8.101): icmp_seq=1 ttl=103 time=48.4 ms
64 bytes from tb-in-f101.1e100.net (142.251.8.101): icmp_seq=2 ttl=103 time=48.4 ms

tcip_ip_np/chapter8$ ./gethostbyaddr 142.251.8.101

official name: tb-in-f101.1e100.net
addr type: IPV4
addr 1: 142.251.8.101
```

