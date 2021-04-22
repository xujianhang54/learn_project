#include <semaphore.h>

int sem_init (sem_t *sem, int pshared, unsigned int value);

int sem_destroy (sem_t *sem);

int sem_wait (sem_t *sem);

int sem_trywait (sem_t *sem);

int sem_timedwait (sem_t *sem, const struct timespec *abs_timeout);\

int sem_post (sem_t *sem);

int sem_getvalue (sem_t *sem, int *sval);