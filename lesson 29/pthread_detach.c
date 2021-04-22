#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *callback (void *arg) {

    printf ("child thread id: %ld\n", pthread_self());

    return NULL;
}

int
main (int argc, char **argv) {

    pthread_t tid;

    // 1. 创建一个子线程
    int ret = pthread_create (&tid, NULL, callback, NULL);

    if (ret != 0) {

        char *errstr = strerror (ret);
        printf ("error: %s\n", errstr);

    }

    // 输出主线程和子线程的ID
    printf ("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    // 设置子线程分离，子线程分离后，子线程的资源在结束时就不需要主线程释放
    pthread_detach (tid);

    pthread_exit (NULL);

    return 0;
}
