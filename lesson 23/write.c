/* 往fifo中写入数据  write.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int
main (int argc, char **argv) {

    char buf [1024];

    // 判断fifo文件是否存在
    int ret = access ("fifo1", F_OK);

    if (ret == -1) {

        printf ("管道不存在，创建管道\n");

        // 如果不存在，则创建管道文件
        ret = mkfifo ("fifo1", 0664);

        if (ret == -1) {

            perror ("mkfifo");
            exit (0);

        }

    }

    // 打开管道，以只写的方式打开管道，因为我们只往管道中写入数据
    int fd = open ("fifo1", O_WRONLY);

    if (fd == -1) {

        perror ("open");
        exit (0);

    }

    for (int i = 0; i < 100; i ++) {

        sprintf (buf, "hello, %d\n", i);
        printf ("write data: %s\n", buf);
        write (fd, buf, strlen (buf));
        sleep (2);       

    }

    close (fd);

    return 0;
}