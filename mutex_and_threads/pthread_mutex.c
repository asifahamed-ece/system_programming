#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int shared_counter = 0;  // Shared Global Variable

pthread_mutex_t counter_mutex; // Mutex Declaration for our Counting Job.

void* sensor_update(void *args){
    int thread_id = *(int *)args;  // Cast it into an int and derefernce to pass the value

    // Each thread Increments for the count of 1M times
    for (int i=0; i<1000000; i++){
        // Lock the Mutex 
        pthread_mutex_lock(&counter_mutex);

        shared_counter ++;      // Critical section enclosed by Mutex Lock.

        //Unlock the Mutex
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("Sensor Thread %d: Updated Count to %d\n", thread_id, shared_counter);
    return NULL;
}

int main()
{
    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    // 1. initialize the Mutex
    if(pthread_mutex_init(&counter_mutex, NULL) != 0){
        perror("Mutex Creation Failed!\n");
        return 1;
    }
    printf("[Main]: Starting 2 Sensor threads...\n");

    // 2. Create the Threads
    for(int i=0; i<2; i++){
        pthread_create(&threads[i], NULL, sensor_update, (void *)&thread_ids[i]);
    }

    // 3. Join the Threads after their completion of Tasks.
    for(int i=0; i<2; i++){
        pthread_join(threads[i], NULL);
    }

    // 4. Destroy the Mutex after Used.
    pthread_mutex_destroy(&counter_mutex);

    // 5. Check the final result
    printf("\n[Main] All threads finished.\n");
    printf("[Main] Final Shared Counter Value: %d\n", shared_counter);
    printf("[Main] Expected Value: 200000 (2 threads * 100000 increments)\n");

    return 0;
}