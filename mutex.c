
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
pthread_mutex_t lock; // The Mutex variable
int counter = 0; 

void *mythread(void *arg) {
    char *letter = arg;
    int i;
    printf("%s: begin [addr of i: %p]\n", letter, &i);
    for (i = 0; i < max; i++) {
        pthread_mutex_lock(&lock);   // Start of Critical Section
        counter = counter + 1; 
        pthread_mutex_unlock(&lock); // End of Critical Section
    }
    printf("%s: done\n", letter);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./main <loop_count>\n");
        exit(1);
    }
    max = atoi(argv[1]);
    pthread_t p1, p2;

    pthread_mutex_init(&lock, NULL); // Initialize the lock

    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("main: done\n [counter: %d]\n [should: %d]\n", counter, max*2);
    
    pthread_mutex_destroy(&lock); // Cleanup
    return 0;
}









#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* trythis(void* arg) {
    pthread_mutex_lock(&lock); // Locking the whole job
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);
    for (i = 0; i < (0xFFFFFFFF); i++);
    printf("\n Job %d has finished\n", counter);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void) {
    int i = 0;
    pthread_mutex_init(&lock, NULL);
    while (i < 2) {
        pthread_create(&(tid[i]), NULL, &trythis, NULL);
        i++;
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}





#include <stdio.h>
#include <pthread.h>

pthread_mutex_t serial_mtx;

void* run(void *arg) {
    pthread_mutex_lock(&serial_mtx); // Acquire lock
    static int serial = 0;
    printf("Thread running! %d\n", serial);
    serial++;
    pthread_mutex_unlock(&serial_mtx); // Release lock
    return NULL;
}

#define THREAD_COUNT 10
int main(void) {
    pthread_t t[THREAD_COUNT];
    pthread_mutex_init(&serial_mtx, NULL);
    
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create((t + i), NULL, run, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(t[i], NULL);
    }
    
    pthread_mutex_destroy(&serial_mtx);
    return 0;
}