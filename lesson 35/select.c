// select.c
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

int
main (void) {

    struct sockaddr_in cliaddr;

    // 创建socket
    int lfd = socket (AF_INET, SOCK_STREAM, 0);

    if (lfd == -1) {
        perror ("socket");
        exit (-1);
    }

    struct sockaddr_in saddr;
    saddr.sin_port = htons (9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl (INADDR_ANY);

    // 绑定
    int ret = bind (lfd, (struct sockaddr *) &saddr, sizeof (saddr));

    if (ret == -1) {
        perror ("bind");
        exit (-1);
    }

    // 监听
    ret = listen (lfd, 20);

    if (ret == -1) {
        perror ("listen");
        exit (-1);
    }

    // 创建一个fd_set的集合，其中存放的是需要检测的文件描述符
    fd_set rd_set, tmp;
    FD_ZERO (&rd_set);
    FD_SET (lfd, &rd_set);
    int maxfd = lfd;

    while (1) {

        tmp = rd_set;

        // 调用select系统调用，让内核帮助检测哪些文件描述符有数据
        ret = select (maxfd + 1, &tmp, NULL, NULL, NULL);

        if (ret == -1) {
            perror ("select");
            exit (1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {

            // 检测到有文件描述符的对应的缓冲区发生了改变

            if (FD_ISSET (lfd, &tmp)) {

                socklen_t len = sizeof (cliaddr);

                // 有新连接
                int cfd = accept (lfd, (struct sockaddr *) &cliaddr, &len);

                // 将新的文件描述符加入到文件描述符集合中
                FD_SET (cfd, &rd_set);

                // 更新最大的文件描述符
                maxfd = (maxfd > cfd) ? maxfd : cfd;

            }

            for (int i = lfd + 1; i <= maxfd; i ++) {

                if (FD_ISSET (i, &tmp)) {

                    // 说明客户端发来了数据
                    char buf [1024] = {0};
                    int len = read (i, buf, sizeof (buf));

                    if (len == -1) {
                        perror ("read");
                        exit (-1);
                    } else if (len == 0) {

                        // 对方断开了连接
                        printf ("Client closed...\n");
                        close (i);
                        FD_CLR (i, &rd_set);

                    } else {

                        // 说明读取到了数据，将数据打印出来
                        printf ("read buf: %s\n", buf);

                        // 向客户端回写数据
                        write (i, buf, strlen (buf) + 1);

                    }

                }

            }

        }

    }

    close (lfd);

    return 0;
}
