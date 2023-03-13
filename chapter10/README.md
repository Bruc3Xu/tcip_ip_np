# 多进程服务器
并发服务器的实现方法：
- 多进程服务器
- 多路复用服务器
- 多线程服务器

## 进程
进程：程序加载进内存执行就成为了进程。进程有对应的进程号PID，操作系统启动后的首个进程`/init`PID为1。

```c
#include <unistd.h>

pid_t fork(void);
// 父进程返回子进程id，子进程返回0
```
子进程共享父进程的内存空间，但修改变量会复制内存空间，二者是独立的。

僵尸进程：子进程结束运行，内核不会立即释放该进程，以满足父进程后续对于子进程退出信息的查询。在子进程结束运行之后，父进程读取其推出状态之前，称之为僵尸态。父进程的结束或者异常退出也会导致子进程进入僵尸态。

销毁僵尸进程：
1. 利用wait函数：父进程主动请求子进程的返回值。
```c
#include <sys/wait.h>
pid_t wait(int* statloc);
// succ=pid, err=-1
// 子进程终止传递的返回值（exit、return）将保存到statloc指向的内存空间
// 使用WIFEXITED(statloc)来判断是否是正常终止
// 使用WEXITSTATUS(statloc)来获得子进程的返回值
```
2. 使用waitpid函数，wait函数会阻塞。
```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int* statloc, int options);
// pid：等待终止的子进程id，传递-1则与wait函数相同，可以等待任意子进程终止。
// statloc同上
// options：传递WNOHANG，即使没有终止的子进程也不会进入阻塞状态，而是返回0并退出函数。
```
3. 使用信号处理来消灭僵尸进程。信号是用户、系统护着进程发送给目标进程的信息，以通知目标进程某个状态的改变。首先是注册信号，使用下面的信号注册函数：
```c
#include <signal.h>
void (*signal)(int signo, void (*func)(int)))(int);
// signo：发生的信号
// func：回调函数
// SIGALAM：已到通过调用alarm函数注册的时间
// SIGINT：输入CTRL+C终止
// SIGCHLD：子进程终止

void mychild(int signo);
signal(SIGCHLD, mychild);
```
### sigaction
sigaction是符合POSIX标准的函数，相较于signal更加通用。
```c
#include <signal.h>

int sigaction(int signo, const struct sigaction* act, struct sigaction* oldact);
// succ = 0, err = -1
// act：信号回调函数
// oldact：获取之前注册的信号处理函数指针，可传0

struct sigaction{
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
};
```




## 基于多任务的并发服务器
基本思想：有新的连接时，创建子进程来处理客户端的连接并提供服务。
1. 服务端父进程调用accept函数来接受连接请求。
2. 创建子进程，将获取的套接字文件描述符传递给子进程（其实是共享的，文件描述符引用数加1）。
2. 子进程利用传递的文件描述符来提供服务。


