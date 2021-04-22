#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *callback (void *arg) {

    printf ("child thread id: %ld\n", pthread_self());

    return NULL;
}

int
main (int argc, char **argv) {

    // 创建一个子线程
    pthread_t tid;

    int ret = pthread_create (&tid, NULL, callback, NULL);

    if (ret != 0) {

        char * errstr = strerror (ret);
        
        printf ("%s\n", errstr);

    }

    // 主线程
    for (int i = 0; i < 5; i ++) {

        printf ("%d\n", i);

    }

    printf ("child thread id: %ld, main thread id: %ld\n", tid, pthread_self ());

    // 当主线程退出时，不会影响到其他正在运行的子线程
    pthread_exit (NULL);

    return 0;
}