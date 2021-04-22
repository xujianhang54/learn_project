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

    // 创建一个线程属性变量
    pthread_attr_t attr;
    int err = pthread_attr_init (&attr);

    if (err != 0) {

        char *errstr = strerror (err);
        printf ("error: %s\n", errstr);

    }

    // 设置属性
    err = pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    size_t size;

    // 1. 创建一个子线程
    int ret = pthread_create (&tid, &attr, callback, NULL);

    if (ret != 0) {

        char *errstr = strerror (ret);
        printf ("error: %s\n", errstr);

    }

    // 获取线程的栈的大小
    pthread_attr_getstacksize (&attr, &size);

    printf ("thread stack size: %ld\n", size);

    // 输出主线程和子线程的ID
    printf ("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    // 释放线程属性资源
    pthread_attr_destroy (&attr);
    
    pthread_exit (NULL);

    return 0;
}
