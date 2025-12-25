#define _XOPEN_SOURCE 600 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

#define NUM_THREADS 6 

pthread_barrier_t barrier; 

void *thread_function(void *arg) { 
    int my_number = *(int *)arg; 
    int result; 
    
    for (int i = 0; i < 3; i++) { // Reduced to 3 iterations for brevity
        fprintf(stderr, "Thread %d: reaching barrier, iteration %d\n", my_number, i); 
        
        // --- BARRIER WAIT ---
        result = pthread_barrier_wait(&barrier); 
        
        // PTHREAD_BARRIER_SERIAL_THREAD is a special return for only one thread
        if(result != 0 && result != PTHREAD_BARRIER_SERIAL_THREAD) { 
            perror("Could not wait on barrier\n"); 
            exit(-1); 
        } 
        fprintf(stderr, "Thread %d: passed barrier!\n", my_number);
    } 
    printf("Bye from %d\n", my_number); 
    pthread_exit(NULL); 
} 

int main() { 
    pthread_t a_thread[NUM_THREADS]; 
    int thread_id[NUM_THREADS]; 

    // 1. BARRIER INIT: Requires 6 threads to trigger release
    if(pthread_barrier_init(&barrier, NULL, NUM_THREADS)) { 
        perror("Could not create a barrier\n"); 
        exit(EXIT_FAILURE); 
    } 

    for(int i = 0; i < NUM_THREADS; i++) { 
        thread_id[i] = i; 
        pthread_create(&(a_thread[i]), NULL, thread_function, (void *)&thread_id[i]); 
    } 

    for(int i = 0; i < NUM_THREADS; i++) { 
        pthread_join(a_thread[i], NULL); 
    } 

    // 2. BARRIER DESTROY
    pthread_barrier_destroy(&barrier);
    printf("All done\n"); 
    return 0; 
}







#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 5
int buffer[MAX];
int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// --- CONSUMER FUNCTION ---
void* consumer(void* arg) {
    while(1) {
        pthread_mutex_lock(&lock);

        // Wait until the buffer is full (count reaches 5)
        while (count < MAX) {
            printf("Consumer: Buffer not full (%d/%d). Sleeping...\n", count, MAX);
            pthread_cond_wait(&cond, &lock);
        }

        // Critical Section: Process the data
        int sum = 0;
        printf("Consumer: Awake! Summing numbers: ");
        for(int i = 0; i < MAX; i++) {
            printf("%d ", buffer[i]);
            sum += buffer[i];
        }
        printf("\nConsumer: Total Sum = %d\n", sum);

        // Reset the buffer for the producer
        count = 0;
        printf("Consumer: Buffer reset. Going back to sleep.\n\n");

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// --- PRODUCER FUNCTION ---
void* producer(void* arg) {
    while(1) {
        int n;
        printf("Producer: Enter a number: ");
        scanf("%d", &n);

        pthread_mutex_lock(&lock);

        // Add item to buffer
        buffer[count++] = n;
        
        // If buffer is full, signal the consumer
        if (count == MAX) {
            printf("Producer: Buffer Full! Signaling Consumer...\n");
            pthread_cond_signal(&cond);
        }

        pthread_mutex_unlock(&lock);
        
        // Small sleep to keep the console output readable
        if (count == 0) usleep(100000); 
    }
    return NULL;
}

int main() {
    pthread_t prod_tid, cons_tid;

    // Create the two specialized threads
    pthread_create(&cons_tid, NULL, consumer, NULL);
    pthread_create(&prod_tid, NULL, producer, NULL);

    // Wait for them (or let them run infinitely)
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}






