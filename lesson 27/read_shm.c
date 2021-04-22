/* read_shm.c */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int
main (int argc, char **argv) {

    // 1. 获取一个共享内存
    int shmid = shmget (100, 0, IPC_CREAT);

    // 2. 将共享内存和当前进程进行关联
    void *ptr = shmat (shmid, NULL, 0);

    // 3. 从共享内存中读取数据
    printf ("%s", (char *) ptr);

    printf ("按任意键继续...\n");
    getchar ();

    // 4. 解除关联
    shmdt (ptr);

    // 5. 删除共享内存
    shmctl (shmid, IPC_RMID, NULL);

    return 0;
}