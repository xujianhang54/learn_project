/* 使用内存映射来完成文件复制的功能 */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main (int argc, char **argv) {

    // 1. 对原始文件进行内存映射
    int fd = open ("english.txt", O_RDWR);

    if (fd == -1) {

        perror ("open");
        exit (0);

    }

    // 2. 创建一个新文件并扩展该文件
    int fd1 = open ("cpy.txt", O_RDWR | O_CREAT, 0664);

    if (fd1 == -1) {

        perror ("open");
        exit (0);

    }

    // 获取原始文件的大小
    int len = lseek (fd, 0, SEEK_END);

    // 对新创建的文件进行扩展
    truncate ("cpy.txt", len);
    write (fd1, " ", 1);   

    // 3. 分别进行内存映射
    void *ptr = mmap (NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void *ptr1 = mmap (NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if (ptr == MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    if (ptr1 = MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    // 内存拷贝
    memcpy (ptr1, ptr, len);

    // 释放资源
    munmap (ptr, len);
    munmap (ptr1, len);

    close (fd1);
    close (fd);

    return 0;
}
