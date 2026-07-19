// shm_writer.c: Writes sensor data to shared memory

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

// Define a Struct for Shared Data
struct sensor_data{
    float temp;
    float humid;
    char status[50];
};

// Union for semctl() (Req by POSIX)
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Semaphore Operations/
void semaphore_wait(int semid){
    struct sembuf op = {0, -1, 0};  // Decrement (Lock)
    if (semop(semid, &op, 1) == -1){
        perror("semop wait failed!\n");
        exit(1);
    }
}

void semaphore_signal(int semid){
    struct sembuf op = {0, 1, 0}; // Increment (Unlock)
    if (semop(semid, &op, 1) == -1){
        perror("semop signal failed!\n");
        exit(1);
    }
}

int main()
{
    key_t shm_key = 12345;
    key_t sem_key = 54321;
    int shmid, semid;
    struct sensor_data *shared_data;
    union semun sem_arg;

    // 1. Create Shared Memeory
    shmid = shmget(shm_key, sizeof(struct sensor_data), IPC_CREAT | 0666);
    if(shmid == -1){
        perror("shmget failed!\n");
        return 1;
    }
    printf("[Writer]: Shared Memory Created with ID: %d\n", shmid);

    // 2. Attach Shared Memory
    shared_data = (struct sensor_data*)shmat(shmid, NULL, 0);
    if (shared_data == (struct sensor_data*)-1){
        perror("shmat failed!\n");
        return 2;
    }
    printf("[Writer]: Shared Memory Attached at address %p.\n", shared_data);

    // 3. Create the Semaphore
    semid = semget(sem_key, 1, IPC_CREAT | 0666);
    if(semid == -1){
        perror("semget failed!\n");
        return 1;
    }
    printf("[Writer]: Semaphore Created with ID: %d\n", semid);

    // 4. Initialize Semaphore to 1 (unlocked state)
    sem_arg.val = 1;
    if(semctl(semid, 0, SETVAL, sem_arg) == -1){
        perror("semctl init failed!\n");
        return 1;
    }
    printf("[Writer]: Semaphore initialized to 1\n");

    // 5. Writing Data in Loop
    for(int i=0; i<5; i++){
        printf("[Writer]: Attempting to Lock semaphore...\n");
        semaphore_wait(semid);

        printf("[Writer]: Semaphore locked. Writing data...\n");
        shared_data->temp = 33.6;
        shared_data->humid = 72.7;
        snprintf(shared_data->status, sizeof(shared_data->status), "Reading #%d", i+1);

        printf("[Writer] Data Written: Temp=%.2f, Humid=%.2f, Status=%s\n", 
            shared_data->temp, shared_data->humid, shared_data->status);

        semaphore_signal(semid);  // Giving back semaphore
        printf("[Writer]: Semaphore Unlocked!\n");
        sleep(2);  // Wait 2 seconds bw each Sensor Write
    }

    // 6. Cleanup
    printf("[Writer]: Detaching Shared memory...\n");
    shmdt(shared_data);

    printf("[Writer]: Removing shared memory...\n");
    shmctl(shmid, IPC_RMID, NULL);

    printf("[Writer]: Removing Semaphore...\n");
    semctl(semid, 0, IPC_RMID);

    printf("[Writer]: CleanUp complete! Exiting...\n");
    return 0;
}
