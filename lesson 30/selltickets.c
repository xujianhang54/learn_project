/* 
    使用多线程实现卖票的案例 
    有3个窗口，一共一百张票，我们模拟并发去卖这100张票
*/
#include <stdio.h>
#include <pthread.h>

int tickets = 100;

void *sellticket (void *arg) {

    // 卖票

    while (tickets > 0) {
        printf ("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
        tickets --;
    }

    return NULL;
}

int
main (int argc, char **argv) {

    // 创建3个子线程
    pthread_t tid1, tid2, tid3;

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


    // 退出主线程
    pthread_exit (NULL);

    return 0;
}