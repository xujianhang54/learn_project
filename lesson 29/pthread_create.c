#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *callback (void *arg) {

    printf ("child thread...\n");
    printf ("arg: %d\n", *(int *) arg);

    return NULL;
}

int
main (int argc, char **argv) {

    pthread_t tid;
    int n = 10;

    // 创建一个子线程
    int ret = pthread_create (&tid, NULL, callback, (void *) &n);

    if (ret != 0) {
        char *errstr = strerror (ret);
        printf ("error: %s\n", errstr);
    }

    for (int i = 0; i < 5; i ++) {

        printf ("%d\n", i);

    }

    pthread_join (tid, NULL);

    return 0;
}