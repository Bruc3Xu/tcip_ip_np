# 多播与广播
## 多播（multicast）
多播可以同时向多个主机传递数据，但这些数据必须是在特定的多播组，称之为组播更合适。
数据传输特点：
- 组播服务器针向组内发送一次数据。
- 组内的所有客户端都会接收到数据。
- 组播的组数可在IP地址范围内随意增加。
- 加入特定组即可接收组内的数据。

组播地址：D类IP地址（224.0.0.0～239.255.255.255）。

组播基于UDP，数据包的格式与UDP数据报相同，只是路由器针对组播数据报会复制并传递到多个主机（要求路由器支持组播），主要用于多媒体数据实时传输（实时、大流量）。

### routing和TTL,以及加入组的方法
TTL是数据的生存时间，TTL用整数表示，每经过一个路由器就减去1,TTL为0时，数据包就无法再被传递。
```c
int sock = scoket(PD_INET, SOCK_DGRAM, 0);
int ttl = 64;
setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));


int recv_sock;
struct ip_mreq join_adr;
recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
join_adr.imr_multiaddr.s_addr = "组播地址";
join_adr.imr_interface.s_addr = "加入组播组的主机IP地址信息";
setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

struct ip_mreq{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};
```
