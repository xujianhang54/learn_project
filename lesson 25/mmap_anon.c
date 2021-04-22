/* 使用内存映射 */

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

int
main (int argc, char **argv) {

    // 1. 创建匿名内存映射区
    int length = 4096;

    void *ptr = mmap (NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    /* 使用匿名映射进行父子进程间的通信 */
    pid_t pid = fork ();

    if (pid > 0) {

        // 父进程
        strcpy ((char *) ptr, "hello world");
        wait (NULL);

    } else {

        sleep (1);

        // 子进程
        printf ("child process: %s\n", (char *) ptr);

    }

    int ret = munmap (ptr, length);

    if (ret == -1) {

        perror ("munmap");
        exit (0);

    }

    return 0;
}