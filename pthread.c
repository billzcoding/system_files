#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 

void *myThreadFun(void *vargp) { 
    sleep(1); 
    printf("Printing GeeksQuiz from Thread \n"); 
    return NULL; 
}   

int main() { 
    pthread_t thread_id; 
    printf("Before Thread\n"); 
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    pthread_join(thread_id, NULL); 
    printf("After Thread\n"); 
    exit(0); 
}






#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 

int g = 0; // Global variable shared by ALL threads

void *myThreadFun(void *vargp) { 
   int *myid = (int *)vargp; 
   static int s = 0; // Static variable shared by threads in this function
   ++s; ++g; 
   printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g); 
} 

void *myThreadFun1(void *vargp) { 
   int *myid = (int *)vargp; 
   static int s = 0; 
   ++s; ++g; 
   printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g); 
} 

int main() { 
   int i; 
   pthread_t tid; 
   for (i = 0; i < 3; i++) 
      pthread_create(&tid, NULL, myThreadFun, (void *)&tid); 
   for (i = 0; i < 3; i++) 
      pthread_create(&tid, NULL, myThreadFun1, (void *)&tid); 

   pthread_exit(NULL); 
   return 0; 
}









#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 

typedef struct data { 
    int* arr; 
    int thread_num; 
} data; 

int arrSize = 10; 

void* halfSum(void* p) { 
    data* ptr = (data*)p; 
    int n = ptr->thread_num; 
    int* thread_sum = (int*)malloc(sizeof(int)); 
    *thread_sum = 0;

    if(n == 0) { 
        for(int i = 0; i < arrSize/2; i++) *thread_sum += ptr->arr[i]; 
    } else { 
        for(int i = arrSize/2; i < arrSize; i++) *thread_sum += ptr->arr[i]; 
    } 
    pthread_exit(thread_sum); 
} 

int main(void) { 
    int* int_arr = (int*)calloc(arrSize, sizeof(int)); 
    for(int i = 0; i < arrSize; i++) int_arr[i] = i; 

    pthread_t tid[2]; 
    data thread_data[2]; 

    for(int i = 0; i < 2; i++) {
        thread_data[i].arr = int_arr;
        thread_data[i].thread_num = i;
        pthread_create(&tid[i], NULL, halfSum, &thread_data[i]);
    }

    int total_sum = 0;
    for(int i = 0; i < 2; i++) {
        int* res;
        pthread_join(tid[i], (void**)&res); 
        total_sum += *res;
        free(res); 
    }
    printf("Total Sum: %d\n", total_sum);
    return 0; 
}







#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sz = 100;
int num_threads = 4;

typedef struct arguments {
    int st_ind;
    int en_ind;
    int* A;
    int iter;
} arguments;

void *thread_worker(void *args1) {
    struct arguments *args = args1;
    int sum = 0;
    for (int i = args->st_ind; i < args->en_ind; i++) {
        sum += args->A[i];
    }
    printf("The sum from thread %d is %d \n", args->iter, sum);
    int *val = malloc(sizeof(int));
    *val = sum;
    return val;
}

int main() {
    pthread_t t_id[num_threads];
    int* Arr = (int*) calloc(sz, sizeof(int));
    for (int i = 0; i < sz; i++) Arr[i] = i + 1;

    arguments args[num_threads];
    for (int i = 0; i < num_threads; i++) {
        args[i].st_ind = i * (sz / num_threads);
        args[i].en_ind = (i + 1) * (sz / num_threads);
        args[i].A = Arr;
        args[i].iter = i;
        pthread_create(&t_id[i], NULL, thread_worker, &args[i]);
    }

    int global_sum = 0;
    for (int i = 0; i < num_threads; i++) {
        int *res;
        pthread_join(t_id[i], (void**)&res);
        global_sum += *res;
        free(res);
    }
    printf("Total Global Sum: %d\n", global_sum);
    return 0;
}





#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef struct {
    int a;
    int b;
} myarg_t;

void *mythread(void *arg) {
    myarg_t *args = (myarg_t *) arg;
    printf("%d %d\n", args->a, args->b);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    myarg_t args = { 10, 20 }; // Example: setting port 8001 here if needed
    int rc = pthread_create(&p, NULL, mythread, &args);
    assert(rc == 0);
    (void) pthread_join(p, NULL);
    printf("done\n");
    return 0;
}