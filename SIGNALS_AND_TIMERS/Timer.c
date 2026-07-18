// We are going to write a program that sets up a repeating timer (like a heartbeat) that fires exactly 
// every 2 second. After it fires 5 times, the program will cleanly disable the timer and exit.
//  This proves we can start, handle, and stop a hardware-like timer entirely in software.

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

// Global flags to communicate between the signal handler and main loop
// 'volatile' prevents the compiler from optimizing this into a CPU register,
// ensuring the main loop always reads the fresh value from RAM.
// 'sig_atomic_t' ensures the read/write is atomic (won't be torn by an interrupt).
volatile sig_atomic_t timer_active = 1;
volatile sig_atomic_t tick_count = 0;

// Signal Handler for SIGALRM (Triggered by ITIMER_REAL expiration)
void timer_handler(int signum){
    tick_count ++;
    printf("[Handler]: Tick! Timer Fired. Count is %d now.\n", tick_count);

    // Stop the Timer after Ticking for 5 Times (10 seconds)
    if (tick_count >= 5){
        printf("[Handler]: Ticks Completed! Stopping Timer...\n");
        timer_active = 0;

        struct itimerval disable_timer = {{0, 0}, {0, 0}};
        setitimer(ITIMER_REAL, &disable_timer, NULL);
        printf("[Handler]: Timer Disabled.\n");
    }
    return;
}   

int main()
{
    printf("-----PERIODIC TIMER FOR EVERY 2 SECONDS-----\n\n");

    // 1, Register the Signal for SIGALRM 
    if (signal(SIGALRM, timer_handler) == SIG_ERR){
        perror("SIGALRM Failed!\n");
        return 1;
    }

    // 2. Configuring the Timer Structure
    struct itimerval mytimer;

    // First Time Initial Timer
    mytimer.it_value.tv_sec = 2;
    mytimer.it_value.tv_usec = 0;

    // Periodic Timer Value (Auto Reload Value)
    mytimer.it_interval.tv_sec = 2;
    mytimer.it_interval.tv_usec = 0;

    printf("[Main]: Starting Repeated Timer (2 Seconds)\n");
    // 3. Arm the Timer
    if ( setitimer(ITIMER_REAL, &mytimer, NULL) == -1){
        perror("Periodicty Error!\n");
        return 2;
    }

    //  4. Simulating some Work by Main
    printf("[Main]: Doing Other Works while the Timer Runs in Background...\n");

    while(timer_active){
        sleep(2); // Sleeping is my Work !
    }

    printf("[Main]: Completed the Given work. Exiting Cleanly.\n");
    return 0;
}