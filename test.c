
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;

void *routine() {
	mails++;
}

int main(int argc, char *argv[])
{
	pthread_t t1, t2;

	if(pthread_create(&t1, NULL, &routine, NULL) != 0){
		return 1;
	}
	if(pthread_create(&t2, NULL, &routine, NULL) != 0){
		return 2;
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Number of mails %d\n", mails);

	return 0;
}