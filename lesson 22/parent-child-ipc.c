/* 实现 ps -aux，使用管道进行父进程和子进程之间的通信 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main (int argc, char **argv) {

    // 创建一个管道
    int pipefd [2];
    int ret = pipe (pipefd);

    if (ret == -1) {

        perror ("pipe");
        exit (0);

    }

    // 创建子进程
    pid_t pid = fork ();

    if (pid > 0) {

        close (pipefd [1]);

        // 父进程
        // 从管道中读取数据
        // 过滤数据（此处不过滤），然后输出

        // 从管道中读取数据
        char buf [1024] = {0};

        int status;

        wait (&status);

        while ((ret = read (pipefd [0], buf, sizeof (buf) - 1)) > 0) {
            printf ("%s", buf);
            memset (buf, 0, sizeof (buf));
        }

    } else if (pid == 0) {

        // 关闭管道的读端
        close (pipefd [0]);

        // 子进程
        // 文件描述符的重定向：将stdout_fileno重定向到管道的写端：fd[1]
        // 执行 ps -aux
        dup2 (pipefd[1], STDOUT_FILENO);

        // 执行ps -aux
        execlp ("ps", "ps", "aux", (char *) 0);
        perror ("execlp");
        exit (0);

    } else {

        perror ("fork");
        exit (0);

    }

    return 0;
}
