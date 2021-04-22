#include <stdio.h>
#include <arpa/inet.h>

int
main (void) {

    // htons：常用来转换端口
    unsigned short a = 0x0102;
    unsigned short b = htons (a);

    printf ("a: %x\n", a);
    printf ("b: %x\n", b);

    printf ("=================================================\n");

    // htonl：常用来转换IP
    char buf [4] = {192, 168, 1, 100};
    
    int num = *(int *) buf;
    int sum = htonl (num);

    unsigned char *p = (char *) &sum;

    printf ("%d %d %d %d\n", p[0], p[1], p[2], p[3]);

    printf ("=================================================\n");

    unsigned char buf1 [4] = {1, 1, 168, 192};
    int num1 = *(int *) buf1;
    int sum1 = ntohl (num1);

    unsigned char *p1 = (unsigned char *) &num1;

    printf ("%d %d %d %d\n", p1[0], p1[1], p1[2], p1[3]); 

    return 0;
}