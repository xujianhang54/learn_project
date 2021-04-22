/* 使用内存映射来完成文件复制的功能 */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void mmapcopy (int src_fd, size_t src_len, int dst_fd);

int
main (int argc, char **argv) {

    // 1. 对原始文件进行内存映射
    int fd = open ("english.txt", O_RDWR);

    if (fd == -1) {

        perror ("open");
        exit (0);

    }

    // 2. 创建一个新文件并扩展该文件
    int fd1 = open ("cpy.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    if (fd1 == -1) {

        perror ("open");
        exit (0);

    }

    // 获取原始文件的大小
    int len = lseek (fd, 0, SEEK_END);

    // 对新创建的文件进行扩展
    truncate ("cpy.txt", len);
    //write (fd1, " ", 1);   

    mmapcopy (fd, len, fd1);
    
    close (fd1);
    close (fd);

    return 0;
}

void mmapcopy (int src_fd, size_t src_len, int dst_fd) {

    void *src_ptr = NULL;
    void *dst_ptr = NULL;

    src_ptr = mmap (NULL, src_len, PROT_READ, MAP_SHARED, src_fd, 0);
    dst_ptr = mmap (NULL, src_len, PROT_READ | PROT_WRITE, MAP_SHARED, dst_fd, 0);

    if (src_ptr = MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    if (dst_ptr == MAP_FAILED) {

        perror ("mmap");
        exit (0);

    }

    // 内存拷贝
    memcpy (dst_ptr, src_ptr, src_len);

    // 释放资源
    munmap (src_ptr, src_len);
    munmap (dst_ptr, src_len);

}