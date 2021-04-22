#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <pthread.h>

struct sockinfo {

    int fd;                     // 通信需要使用的文件描述符
    struct sockaddr_in addr;    // 客户端的socket信息
    pthread_t tid;              // 线程号
    
};

struct sockinfo sockinfos [128];

void *working (void *arg) {

    // 子线程与客户端通信
    // 必须要有cfd，客户端的信息，线程号

    struct sockinfo *pinfo = (struct sockinfo *) arg;

    char cliIp [16];
    inet_ntop (AF_INET, &pinfo -> addr.sin_addr.s_addr, cliIp, sizeof (cliIp));
    unsigned short cliPort = ntohs (pinfo -> addr.sin_port);

    printf ("client IP is %s, port is %d\n", cliIp, cliPort);

    // 接收客户端发来的数据
    char recvbuf [1024];

    while (1) {
        int len = read (pinfo -> fd, recvbuf, sizeof (recvbuf));

        if (len == -1) {
            perror ("read");
            exit (-1);
        } else if (len > 0) {
            printf ("recv client data: %s\n", recvbuf);
        } else if (len == 0) {
            printf ("client closed. \n");
            break;
        }

        write (pinfo -> fd, recvbuf, strlen (recvbuf) + 1);
    }

    close (pinfo -> fd);

    return NULL;
}

int
main (void) {

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

    // 初始化数据
    int max = sizeof (sockinfos) / sizeof (sockinfos [0]);

    for (int i = 0; i < max; i ++) {

        bzero (&sockinfos[i], sizeof (sockinfos [i]));
        sockinfos [i].fd = -1;
        sockinfos [i].tid = -1;

    }


    // 循环等待客户端的连接，一旦一个客户端连接进来，就创建一个子线程来处理通信
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

        struct sockinfo *pinfo = NULL;
        
        for (int i = 0; i < max; i ++) {

            // 从数组中找到一个可用的sockinfo元素
            if (sockinfos [i].fd == -1) {
                pinfo = &sockinfos [i];
                break;
            }

            if (i == max - 1) {
                sleep (1);
                i --;
            }

        }

        pinfo -> fd = cfd;
        memcpy (&pinfo -> addr, &cliaddr, len);

        // 创建子线程
        pthread_t tid;
        pthread_create (&pinfo -> tid, NULL, working, (void *) pinfo);

        pthread_detach (pinfo -> tid);

    }

    close (lfd);

    return 0;
}