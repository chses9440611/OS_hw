#define _GNU_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


extern int spin_lock(int* mutex);
extern int spin_unlock(int* mutex);


int cnt=0;
int mutex=0;

unsigned int niters;

int thread_proc[8] = {1, 2}; 

void *thread(void *vargp) {
    unsigned long* mask = vargp;
    
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), (void*)mask ) == -1){
        perror("Failed to set pthread affinity");
        return NULL;
    }

    for (long i = 0; i < niters; i++){
        spin_lock(&mutex);
        cnt++;
        spin_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t tid[2];
    
    if(argc < 2){
        fprintf(stderr, "Usage: ./spinlock 10000\n");
        exit(1);
    }

    niters = atoi(argv[1]);
    
    //printf("%d CPUs can be used \n", sysconf(_SC_NPROCESSORS_CONF));

    for(int i=0; i<2; i++)
        pthread_create(&(tid[i]), NULL, thread, &(thread_proc[i]));
    for(int i=0; i<2; i++)
        pthread_join(tid[i], NULL);

    if (cnt != (2 * niters))
    {
        printf("Wrong! cnt=%d\n", cnt);
        exit(1);
    }
    else
        printf("Correct! cnt=%d\n", cnt);
    exit(0);
}

