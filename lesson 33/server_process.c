// server_process.c
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recycleChild (int argc) {

    while (1) {

        int ret = waitpid (-1, NULL, WNOHANG);

        if (ret == -1) {

            // 所有的子进程都回收完了
            break;

        } else if (ret == 0) {

            // 还有子进程活着
            break;

        } else if (ret > 0) {

            // 被回收了
            printf ("子进程 %d 被回收了...\n", ret);

        }

    }

}

int
main (void) {

    // 注册信号捕捉
    struct sigaction act;

    act.sa_flags = 0;
    sigemptyset (&act.sa_mask);
    act.sa_handler = recycleChild;

    sigaction (SIGCHLD, &act, NULL);

    // 1. 创建socket
    int lfd = socket (AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    if (lfd == -1) {
        perror ("socket");
        exit (-1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (9999);
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

    socklen_t len = sizeof (servaddr);

    // 2. 绑定
    int ret = bind (lfd, (struct sockaddr *) &servaddr, len);

    if (ret == -1) {
        perror ("bind");
        exit (-1);
    }

    // 3. 监听
    ret = listen (lfd, 20);

    if (ret == -1) {
        perror ("listen");
        exit (-1);
    }

    // 不断循环，等待客户端连接
    while (1) {

        socklen_t len = sizeof (cliaddr);

        // 接收连接
        int cfd = accept (lfd, (struct sockaddr *) &cliaddr, &len);

        if (cfd == -1) {

            if (errno == EINTR) {
                continue;
            }

            perror ("accept");
            exit (-1);
        }

        // 每一个连接进来，就创建一个子进程，使用子进程来跟客户端进行通信
        pid_t pid = fork ();

        if (pid == 0) {

            // 子进程
            // 获取客户端的信息
            char cliIp [16];
            inet_ntop (AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof (cliIp));
            unsigned short cliPort = ntohs (cliaddr.sin_port);

            printf ("client IP is %s, port is %d\n", cliIp, cliPort);

            // 接收客户端发来的数据
            char recvbuf [1024];

            while (1) {
                int len = read (cfd, recvbuf, sizeof (recvbuf));

                if (len == -1) {
                    perror ("read");
                    exit (-1);
                } else if (len > 0) {
                    printf ("recv client data: %s\n", recvbuf);
                } else if (len == 0) {
                    printf ("client closed. \n");
                    break;
                }

                write (cfd, recvbuf, strlen (recvbuf) + 1);
            }

            close (cfd);
            exit (0);

        }


    }


    return 0;
}