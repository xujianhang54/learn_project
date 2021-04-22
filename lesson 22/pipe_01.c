/* 父子进程通过匿名管道进行通信 */
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main (int argc, char **argv) {

    int pipefd [2];
    char buf [1024] = {0};

    // 在fork之前创建管道
    int ret = pipe (pipefd);

    if (ret  == -1) {
        perror ("pipe");
        exit (0);
    }

    // 创建子进程
    pid_t pid = fork ();

    if (pid > 0) {          // 父进程

        // 从管道的读取端读取数据
        int len = read (pipefd [0], buf, sizeof (buf));

        printf ("child recv: %s, pid = %d\n", buf, getpid ());

    } else if (pid == 0) {  // 子进程

        char *str = "Hello, I am child";

        write (pipefd[1], str, strlen (str));

    }

    return 0;
}
