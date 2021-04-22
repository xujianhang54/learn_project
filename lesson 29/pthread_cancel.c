#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *callback (void *arg) {

    printf ("child thread id: %ld\n", pthread_self());

    for (int i = 0; i < 5; i ++) {
        printf ("child : %d\n", i);
    }

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

    // 取消线程
    pthread_cancel (tid);

    for (int i = 0; i < 5; i ++) {
        printf ("%d\n", i);
    }

    pthread_exit (NULL);

    return 0;
}
