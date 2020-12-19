#include <stdlib.h>
#include <pthread.h>

void *routine(void *param)
{
	pthread_exit(0);
}

int main()
{
	void *p = malloc(128);
	
	pthread_t tid;
 	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, routine, NULL);
	pthread_join(tid,NULL);
	
	return 0;
}
