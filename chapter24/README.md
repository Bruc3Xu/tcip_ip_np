## HTTP
Hypertext Transfer Protocol是以超文本传输为目的而设计的应用层协议。

HTTP本身是无状态的Stateless协议，需要Cookie和Session来识别客户端。

请求消息的结构体：
- 请求行：GET /index.html HTTP/1.1
- 消息头：浏览器、用户认证的等附加信息。
- 空行
- 消息体

相应消息的结构：
- 状态行：请求执行结果
- 消息头：传输的数据类型、长度等附加信息。
- 空行
- 消息体