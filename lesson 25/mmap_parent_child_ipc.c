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

    // 1. 打开一个文件
    int fd = open ("test.txt", O_RDWR);
    // 获取文件的大小
    int size = lseek (fd, 0, SEEK_END);

    // 2. 创建内存映射区
    void *ptr = mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    // 3. 创建子进程
    pid_t pid = fork ();

    if (pid > 0) {

        wait (NULL);

        // 父进程
        char buf [64];
        strcpy (buf, (char *) ptr);
        printf ("read data: %s\n", buf);
        
    } else if (pid == 0) {

        // 子进程
        strcpy ((char *) ptr, "nihao a, son!!!");


    }

    // 关闭内存映射区
    munmap (ptr, size);

    return 0;
}