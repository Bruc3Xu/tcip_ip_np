## 面向连接的套接字（SOCK_STREAM）

有以下特征：
- 可靠：保证数据不会丢失
- 按序：数据顺序到达
- 基于字节的：数据的发送和接收不是同步的或者称传输的数据不存在数据边界

不管数据分几次传送过来，接收端只需要根据自己的要求读取，不用非得在数据到达时立即读取。
传送端有自己的节奏，接收端也有自己的节奏，它们是不一致的。

## 数据报格式套接字（SOCK_DGRAM）

数据报格式套接字（Datagram Sockets）也叫“无连接的套接字”。计算机只管传输数据，不作数据校验，如果数据在传输中损坏，或者没有到达另一台计算机，是没有办法补救的。也就是说，数据错了就错了，无法重传。

因为数据报套接字所做的校验工作少，所以在传输效率方面比流格式套接字要高。

有以下特征：
- 不可靠：传输的数据可能丢失也可能损毁
- 不按顺序传递：强调快速传输而非传输顺序
- 限制每次传输的数据大小
- 数据的发送和接收是同步的