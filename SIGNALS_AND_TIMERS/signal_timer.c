// Sets up a custom handler for SIGINT (Ctrl+C) so it doesn't just quit immediately.
// Sets up a custom handler for SIGALRM.
// Uses alarm(3) to trigger the timer after 3 seconds.
// Uses raise() to send a custom signal to itself (SIGUSR1).

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// Declaring the SignalHandlers for Signal captures
volatile sig_atomic_t keep_running = 1;
volatile sig_atomic_t alarm_triggered = 0; 
// sig_atomic_int ->  only type guaranteed by the language standard to safely communicate flags
//  between a main program and an asynchronous signal handler running within the same thread


// 1. Custom SIGINT handler for Ctrl+C
void sigint_handler(int signum){
    printf("\n[Handler]: Caught SIGINT (%d). Setting Flag to exit.!\n", signum);
    keep_running = 0;
}

// 2. Custom SIGALRM Handler for Timer
void sigalrm_handler(int signum){
    printf("\n[Handler]: Caught SIGALRM (%d). Timer Expired!\n", signum);
    alarm_triggered = 1;
}

// 3, Custom Handler for SIGUSR1 
void sigusr1_handler(int signum){
    printf("\n[Handler]: Caught Custom Signal SIGUSR1 (%d)!\n", signum);
}



int main()
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("SIGINT Failed!\n");
        return 1;
    }
    if(signal(SIGALRM, sigalrm_handler) == SIG_ERR){
        perror("SIGALRM Failed!\n");
        return 1;
    }
    if(signal(SIGUSR1, sigusr1_handler) == SIG_ERR){
        perror("SIGUSR1 Failed!\n");
        return 1;
    }

    // 2. Set alarm for 3 seconds using timer
    printf("[Main]: Setting Alarm for 3 Seconds\n");
    alarm(3);

    // 3. Main Loop simulating some work, we will print ...
    printf("[Main] Doing work... (Press Ctrl+C to trigger SIGINT, or wait for timer).\n");
    long int counter = 0;
    while(keep_running){
        printf(".");
        fflush(stdout); // force push the buffer out
        sleep(1); // buff delay for pushing to screen

        if(alarm_triggered){
            // Send SIGUSR1 when alarm Triggered.
            printf("[Main]: Alarm Triggered! Sending SIGUSR1!\n");
            raise(SIGUSR1); // Sending SIGNAL to itself
            alarm_triggered = 0;

            printf("[Main]: Setting Alarm again for 5 Seconds...\n");
            alarm(5);
        }
        counter ++;
    }

    printf("\n[Main]: Exiting the Program Gracefully!\n");
    printf("Internal Counter Ran for %ld times.\n", counter);
    printf("Good bye...!\n");

    return 0;
}
