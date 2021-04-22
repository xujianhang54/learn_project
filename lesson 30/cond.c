#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct Node {

    int num;
    struct Node *next;

};

struct Node *head = NULL;

pthread_mutex_t mutex;

// 创建条件变量
pthread_cond_t cond;

void *producer (void *arg) {

    // 不断地创建新的节点，然后添加到链表中
    while (1) {
        pthread_mutex_lock (&mutex);
        struct Node *newNode = (struct Node *) malloc (sizeof (struct Node));
        newNode -> next = head;
        head = newNode;
        newNode  -> num = rand () % 1000;
        printf ("add node, num: %d, tid: %ld\n", newNode -> num, pthread_self ());

        // 只要生产了一个就通知消费者去消费
        pthread_cond_signal (&cond);

        pthread_mutex_unlock (&mutex);
        usleep (100);
    }

    return NULL;
}

void *customer (void *arg) {

    while (1) {
        pthread_mutex_lock (&mutex);
        struct Node *tmp = head;

        // 判断是否有数据
        if (head != NULL) {

            // 有数据
            head = head -> next;
            printf ("delete num: %d, tid: %ld\n", tmp -> num, pthread_self ());
            free (tmp);
            pthread_mutex_unlock (&mutex);
            usleep (100);

        } else {

            // 没有数据，需要等待，当这个函数调用阻塞的时候，会对互斥锁进行解锁，当不阻塞的时候，
            // 继续向下执行会重新加锁
            pthread_cond_wait (&cond, &mutex);
            pthread_mutex_unlock (&mutex);

        }

    }

}

int
main (void) {

    pthread_mutex_init (&mutex, NULL);
    pthread_cond_init (&cond, NULL);

    // 创建5个生产者线程和5个消费者线程
    pthread_t ptids [5];
    pthread_t ctids [5];

    for (int i = 0; i < 5; i ++) {
        pthread_create (&ptids [i], NULL, producer, NULL);
        pthread_create (&ctids [i], NULL, customer, NULL);
    }

    for (int i = 0; i < 5; i ++) {
        pthread_detach (ptids [i]);
        pthread_detach (ctids [i]);
    }

    while (1) {
        sleep (10);
    }

    pthread_mutex_destroy (&mutex);
    pthread_cond_destroy (&cond);

    pthread_exit (NULL);

    return 0;
}