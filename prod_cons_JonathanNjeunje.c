#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
//Constants definition:
#define NUM_PROD_THREAD 5
#define NUM_CONS_THREAD 5
#define BUFFER_SIZE 5
#define MAX_EXCHANGE 1000
#define PROD_SPEED 1000000 //Speed of production in micro seconds.
#define CONS_SPEED 4000000 //Speed of consumption in micro seconds.
#define DEBUG true //Activate/deactivate debug lines.
//Declaration of the global variables:
//semaphore
sem_t empty, full;
//mutex
pthread_mutex_t mutexProd, mutexCons;
//Array
int buffer[BUFFER_SIZE];
int prodStep = 0, consStep = 0;

void *prod_thread(void *param) {
	while (1) {
		sem_wait(&empty); //Prevent over-production.
		pthread_mutex_lock(&mutexProd); //Atomizes production.
	
		if (prodStep > MAX_EXCHANGE) { //Limit the number of productions.
			pthread_mutex_unlock(&mutexProd); //Release.
			sem_post(&full); //Permits consumption.
			break;
		}
		
		//Critical Section: Enter: Prod.
		int prodNum = prodStep % BUFFER_SIZE + 1; //Producer number
		/*DEBUG*/if (DEBUG) printf("-> Critical Section: -->Prod#%d\n", prodNum); 
		buffer[prodNum - 1] = prodStep; //Production.
		/*DEBUG*/if (DEBUG) printf("-> Prod#%d: %d\n", prodNum, prodStep);	
		/*DEBUG*/if (DEBUG) printf("-> Critical Section: <--Prod#%d\n", prodNum); 
		prodStep++; //Production tracker.
		usleep(PROD_SPEED); //Sleep to simulate the production speed.
		//Critical Section: Exit: Prod.
			
		pthread_mutex_unlock(&mutexProd); //Release.
		sem_post(&full); //Permits consumption.
	}
	pthread_exit(0);
}

void *cons_thread(void *param) {
	while (1) {
		sem_wait(&full); //Prevent under-consumption.
		pthread_mutex_lock(&mutexCons); //Atomizes consumptions.

		if (consStep > MAX_EXCHANGE) { //Limit the number of consumptions.
			pthread_mutex_unlock(&mutexCons); //Release.
			sem_post(&empty); //Permits production.
			break;
		}

		//Critical Section: Enter: Cons.
		int consNum = consStep % BUFFER_SIZE + 1;
		/*DEBUG*/if (DEBUG) printf("-> Critical Section: -->Cons#%d\n", consNum);
		printf("-> Cons#%d: %d\n", consNum, buffer[consNum - 1]); //Consumption.	
		/*DEBUG*/if (DEBUG) printf("-> Critical Section: <--Cons#%d\n", consNum);
		consStep++; //Consumption tracker.
		usleep(CONS_SPEED); //Sleep to simulate the consumption speed.
		//Critical Section: Exit: Cons.

		pthread_mutex_unlock(&mutexCons); //Release.
		sem_post(&empty); //Permits production.
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]) {
	//Initialize the semaphores.
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	//Initialize the mutexes.
	pthread_mutex_init(&mutexProd, NULL);
	pthread_mutex_init(&mutexCons, NULL);
	//Declare thread ids and attr.
	pthread_t tidProd[NUM_PROD_THREAD], tidCons[NUM_CONS_THREAD];
	pthread_attr_t tattr;
	//Set default thread attributes.
	pthread_attr_init(&tattr);

	int i;	

	for (i = 0; i < NUM_PROD_THREAD; i++) {
		pthread_create(&tidProd[i], &tattr, prod_thread, NULL);
	}
	for (i = 0; i < NUM_CONS_THREAD; i++) {
		pthread_create(&tidCons[i], &tattr, cons_thread, NULL);
	}

	for (i = 0; i < NUM_PROD_THREAD; i++) {
		pthread_join(tidProd[i], NULL);
	}
	for (i = 0; i < NUM_CONS_THREAD; i++) {
		pthread_join(tidCons[i], NULL);
	}

	return 0;	
}

