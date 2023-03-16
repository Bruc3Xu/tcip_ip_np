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
## 广播
广播也可以一次性向多个主机发送数据，但不同于组播，只能向同一个网络中的主机传输数据。
- 直接广播：如向192.12.34中的所有主机发送数据，目的地址设为192.12.34.255（即主机地址位全部设为1）。
- 本地广播：如192.32.24网络中的逐句向255.255.255.255传输数据时，数据传输到网络中的所有主机。

```c
int sock = socket(PF_INET, SOCK_DGRAM, 0);
int bcast = 1;
setsockopt(sock, SL_SOCKET, SO_BROADCAST, (void*)&bcast, sizeof(bcast));
```