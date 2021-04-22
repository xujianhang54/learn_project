/* 从fifo中读取数据  read.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int
main (int argc, char **argv) {

    char buf [1024] = {0};

    // 打开管道文件
    int fd = open ("fifo1", O_RDONLY);

    if (fd == -1) {

        perror ("open");
        exit (0);

    }

    int len = 0;

    // 读取数据
    while ((len = read (fd, buf, sizeof (buf))) > 0) {

        printf ("recv buf: %s\n", buf);

    }

    close (fd);

    return 0;
}