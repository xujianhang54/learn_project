/*
     案例：创建8个线程创建同一个全局变量
     3个进程不定时的写这个全局变量，5个进程不定时的读这个全局变量
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 共享数据
int num = 1;

pthread_rwlock_t rwlock;

void *writeNum (void *arg) {

    while (1) {
        pthread_rwlock_wrlock (&rwlock);
        num ++;
        printf ("++write, tid: %ld, num: %d\n", pthread_self (), num);
        pthread_rwlock_unlock (&rwlock);
        usleep (100);
    }

}

void *readNum (void *arg) {

    while (1) {

        pthread_rwlock_rdlock (&rwlock);
        printf ("=====read, tid: %ld, num = %d\n", pthread_self (), num);
        pthread_rwlock_unlock (&rwlock);
        usleep (100);

    }

}


int
main (void) {

    pthread_rwlock_init (&rwlock, NULL);

    // 创建3个写线程，5个读线程
    pthread_t wtids [3];
    pthread_t rtids [5];

    for (int i = 0; i < 3; i ++) {

        pthread_create (&wtids [i], NULL, writeNum, NULL);

    }

    for (int i = 0; i < 5; i ++) {

        pthread_create (&rtids [i], NULL, readNum, NULL);

    }

    // 设置线程分离
    for (int i = 0; i < 3; i ++) {

        pthread_detach (wtids [i]);

    }

    for (int i = 0; i < 5; i ++) {

        pthread_detach (rtids [i]);

    }

    pthread_rwlock_destroy (&rwlock);

    pthread_exit (NULL);

    return 0;
}