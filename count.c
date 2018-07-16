//grupo
//Lucas Resende de Sousa Amaral
//Cliff Alves
//Railton Gomes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#define NUM_THREADS 50
 
// create thread argument struct
typedef struct _thread_data_t {
  int 		tid;
  double 	stuff;
  int 		*refCount;
  char 		*refStatus;
} thread_data_t;

//mutex var
pthread_mutex_t lockCount;
 
//function of thread
void *thr_func(void *arg) {
  thread_data_t *data 	= (thread_data_t *)arg;
  char 					*status;
  int 					*count;
  
  status = data->refStatus;
  count = data->refCount;
  
  
  if(*status == 's'){
	  //without control
	  *count = *count + 1;
	  printf("Count >> %d\n",*count);
  } else if (*status == 'c') {
	  //with control
	  pthread_mutex_lock(&lockCount);
 		*count = *count + 1;
	    printf("Count >> %d\n",*count);
  	  pthread_mutex_unlock(&lockCount);
  }
  pthread_exit(NULL);
}

//main function
int main(int argc, char **argv) {
  //init vars	   	
  pthread_t thr[NUM_THREADS];
  int i, rc, count = 0;
  char status;
  
   //check if call correct
  if((argc != 2)){
	  printf("make the call correct, put 's' or 'c' for controll");
	  return EXIT_FAILURE;
  } else {
	  status = (char) argv[1][0];
  }
  
  //create a thread_data_t array 
  thread_data_t thr_data[NUM_THREADS];
  //init mutex
  pthread_mutex_init(&lockCount, NULL);
 
  //create threads and call
  for (i = 0; i < NUM_THREADS; ++i) {
    thr_data[i].tid 		= i;
    thr_data[i].refStatus 	= &status;
    thr_data[i].refCount 	= &count;
    //create threads
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  //wait for all threads to finish
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(thr[i], NULL);
  }
 
  return EXIT_SUCCESS;
}
