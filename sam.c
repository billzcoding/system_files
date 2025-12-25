#include <stdio.h>
#include <pthread.h>

int data_ready = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* consumer(void* arg) {
    pthread_mutex_lock(&mtx);
    while(data_ready == 0) { // Wait for signal
        pthread_cond_wait(&cond, &mtx);
    }
    printf("Consumer: Data received from port 8001!\n");
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* producer(void* arg) {
    pthread_mutex_lock(&mtx);
    data_ready = 1;
    pthread_cond_signal(&cond); // Wake up consumer
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main() {
    pthread_t c, p;
    pthread_create(&c, NULL, consumer, NULL);
    pthread_create(&p, NULL, producer, NULL);
    pthread_join(c, NULL); pthread_join(p, NULL);
    return 0;
}










#include <pthread.h>
#include <stdio.h>

int counter = 0;
pthread_mutex_t lock;

void* count_task(void* arg) {
    for(int i=0; i<10000; i++) {
        pthread_mutex_lock(&lock);   // START CRITICAL SECTION
        counter++;
        pthread_mutex_unlock(&lock); // END CRITICAL SECTION
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, count_task, NULL);
    pthread_create(&t2, NULL, count_task, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final Counter: %d (Should be 20000)\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}







#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    if (fork() == 0) {
        // --- For Orphan: Child sleeps while parent dies ---
        printf("Child: My Parent is %d\n", getppid());
        sleep(5);
        printf("Child: My New Parent (Orphaned) is %d\n", getppid());
    } else {
        // --- For Zombie: Parent sleeps while child dies ---
        // exit(0); // Uncomment this to show Orphan
        sleep(2);
        printf("Parent: I didn't wait() yet, child is a Zombie now.\n");
        exit(0);
    }
    return 0;
}










#include <stdio.h>
#include <pthread.h>

#define SIZE 10
#define NUM_THREADS 2

int arr[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int partial_sums[NUM_THREADS] = {0};

void* sum_array(void* arg) {
    int thread_id = *(int*)arg;
    int start = thread_id * (SIZE / NUM_THREADS);
    int end = (thread_id + 1) * (SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        partial_sums[thread_id] += arr[i];
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, sum_array, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total Sum: %d\n", partial_sums[0] + partial_sums[1]);
    return 0;
}











#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <pthread.h>

pthread_barrier_t my_barrier;

void* task(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d: Finished Step 1\n", id);
    
    pthread_barrier_wait(&my_barrier); // Everyone stops here until all threads arrive
    
    printf("Thread %d: Starting Step 2\n", id);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int i1 = 1, i2 = 2;
    
    pthread_barrier_init(&my_barrier, NULL, 2); // Wait for 2 threads

    pthread_create(&t1, NULL, task, &i1);
    pthread_create(&t2, NULL, task, &i2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    pthread_barrier_destroy(&my_barrier);
    return 0;
}


