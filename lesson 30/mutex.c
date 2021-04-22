/* 
    使用多线程实现卖票的案例 
    有3个窗口，一共一百张票，我们模拟并发去卖这100张票
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int tickets = 10000;

// 创建一个互斥量
pthread_mutex_t mutex;



void *sellticket (void *arg) {

    // 卖票

    // 加锁
    while (1) {

        // 加锁
        pthread_mutex_lock (&mutex);

        if (tickets > 0) {
            //usleep (6000);
            printf ("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
            tickets --;
        } else {

            // 解锁
            pthread_mutex_unlock (&mutex);

            break;
        }

        // 解锁
        pthread_mutex_unlock (&mutex);
    }


    return NULL;
}

int
main (int argc, char **argv) {

    // 创建3个子线程
    pthread_t tid1, tid2, tid3;
    // 初始化互斥量
    pthread_mutex_init (&mutex, NULL);


    pthread_create (&tid1, NULL, sellticket, NULL);
    pthread_create (&tid2, NULL, sellticket, NULL);
    pthread_create (&tid3, NULL, sellticket, NULL);

    // 回收子线程的资源
    pthread_join (tid1, NULL);
    pthread_join (tid2, NULL);
    pthread_join (tid3, NULL);

    // 设置线程分离
    //pthread_detach (tid1);
    //pthread_detach (tid2);
    //pthread_detach (tid3);

    // 释放互斥量资源
    pthread_mutex_destroy (&mutex);

    // 退出主线程
    pthread_exit (NULL);

    return 0;
}