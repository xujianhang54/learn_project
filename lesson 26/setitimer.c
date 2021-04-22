/* 使用定时器设置：三秒之后，每隔两秒定时一次 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

int
main (int argc, char **argv) {

    struct itimerval new_value;

    // 设置new_value的值
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer (ITIMER_REAL, &new_value, NULL);

    printf ("定时器开始了...\n");

    if (ret == -1) {

        perror ("setitimer");
        exit (0);

    }

    getchar ();

    return 0;
}