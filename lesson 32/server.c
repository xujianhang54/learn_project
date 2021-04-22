// TCP通信服务器端
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main (int argc, char **argv) {

    // 1. 创建socket，用于监听套接字
    int lfd = socket (AF_INET, SOCK_STREAM, 0);

    if (lfd == -1) {
        perror ("socket");
        exit (0);
    }

    // 2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl (INADDR_ANY);
    saddr.sin_port = htons (9999);
    
    int ret = bind (lfd, (struct sockaddr *) &saddr, sizeof (saddr));

    if (ret == -1) {

        perror ("bind");
        exit (0);

    }

    ret = listen (lfd, 20);

    if (ret == -1) {

        perror ("listen");
        exit (0);

    }

    // 接收客户端的连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof (clientaddr);
    
    int cfd = accept (lfd, (struct sockaddr *) &clientaddr, &len);

    if (cfd == -1) {

        perror ("accept");
        exit (0);

    }

    // 输出客户端的信息
    char clientIp [16];

    inet_ntop (AF_INET, &clientaddr.sin_addr.s_addr, clientIp, sizeof (clientIp));
    unsigned short clientPort = ntohs (clientaddr.sin_port);
    printf ("clientIP: %s, clientPort: %d\n", clientIp, clientPort);

    // 获取客户端的数据，然后给客户端发送数据
    char recvbuf [1024];
    ssize_t rlen = read (cfd, recvbuf, sizeof (recvbuf));

    if (rlen == -1) {

        perror ("read");
        exit (0);

    } else if (rlen > 0) {

        printf ("recv client data: %s\n", recvbuf);

    } else if (rlen == 0) {

        // 客户端断开连接
        printf ("client closed...\n");

    }

    // 给客户端发送数据
    char *data = "hello, i am server"; 
    write (cfd, data, sizeof (data));

    // 关闭文件描述符
    close (cfd);
    close (lfd);

    return 0;
}