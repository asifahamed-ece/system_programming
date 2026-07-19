// shm_reader.c: Reads sensor data from shared memory

#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>

// Same Struct for semsor_data as Writer
struct sensor_data{
    float temp;
    float humid;
    char status[50];
};

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void semaphore_wait(int semid){
    struct sembuf op = {0, -1, 0};      // Decrement semaphore value
    if(semop(semid, &op, 1) == -1){
        perror("semop wait failed!\n");
        exit(1);
    }
}

void semaphore_signal(int semid){
    struct sembuf op = {0, 1, 0};   // Increment the Semaphore value   
    if (semop(semid, &op, 1) == -1){
        perror("semop signal failed!\n");
        exit(2);
    }
}

int main()
{
    key_t shm_key = 12345;
    key_t sem_key = 54321;
    int shmid, semid;
    struct sensor_data *shared_data;

    // 1. Get the existing shared memory created by the shm_writer.c
    shmid = shmget(shm_key, sizeof(struct sensor_data), 0666); // No IPC_CREAT since writer already Created IPC
    if(shmid == -1){
        perror("[Reader]: shmget failed!\n");
        return 1;
    }
    printf("[Reader]: Attached to Shared Memory.\n");

    // 2. Attach shared Memory
    shared_data = (struct sensor_data*)shmat(shmid, NULL, 0);
    if(shared_data == (struct sensor_data*)-1){
        perror("shmat failed!\n");
        return 1;
    }
    printf("[Reader]: Shared memory attached at address %p.\n", shared_data);

    // 3. get existing semaphore.
    semid = semget(sem_key, 1, 0666);
    if(semid == -1){
        perror("[Reader]: semget failed!\n");
        return 1;
    }
    printf("[Reader]: Attached to Semaphore ID: %d\n\n", semid);

    // 4. Read in a Loop
    for(int i=0; i<5; i++){
        printf("[Reader]: Trying to Lock Semaphore...\n");
        semaphore_wait(semid);\

        printf("[Reader]: Semaphore Acquired. Reading data...\n");
        printf("[Reader] Data read: Temp=%.2f, Humid=%.2f, Statsu=%s\n", 
            shared_data->temp, shared_data->humid, shared_data->status);
        
            semaphore_signal(semid); // Unlocking the Semaphore
            printf("[Reader]: Semaphore Unlocked.\n\n");
            sleep(2);   // Small delay bw Reading
    }

    // 5. Detach Shared memory (Reader shd not delete, Writer will delete)
    printf("[Reader]: Detaching Shared Memory...\n");
    shmdt(shared_data);

    printf("[Reader]: Done Reading! Exiting...\n");
    return 0;
}