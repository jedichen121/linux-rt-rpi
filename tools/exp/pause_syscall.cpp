#include <chrono>
#include <thread>
#include <sched.h>
#include <ctime>
#include <csignal>
#include <unistd.h>
#include <sys/syscall.h>

#include <iostream>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN
#define SIG2 SIGRTMIN+1

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int hertz = 10;        // outerloop frequency, hertz.

void test_hi(void)
{
    int i,j;
    for(i = 0; i < 5000000; i++) 
        j=i;
}

static void
handler(int sig, siginfo_t *si, void *uc)
{
    long ms;
    long offset;
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    test_hi();

    clock_gettime(CLOCK_MONOTONIC, &t2);
    ms = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_nsec - t1.tv_nsec) / 1000000;
    offset = (t1.tv_nsec / 1000) % (1000 / hertz * 1000);
    
    // printf("offset: %d, exec: %d\n", offset, ms);
    printf("%d\t%d\n", offset, ms);

    // syscall(400);
}

static void
handler2(int sig, siginfo_t *si, void *uc)
{
    // exit program
    exit(0);
}

int main(int argc, char * argv[])
{
    int priority = 30;      // process real-time priority
    int policy = SCHED_FIFO;
    int run_period = 0;

    struct sched_param param;
    timer_t timer, timer2;
    struct sigevent sev, sev2;
    struct sigaction sa, sa2;
    struct itimerspec its, its2;

    // process input options
    for (size_t i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-hz") {
            // set timer hertz
            if (i + 1 < argc)
            {
                hertz = atof(argv[i + 1]);
            }
        }
        else if (std::string(argv[i]) == "-p") {
            // set process priority
            if (i + 1 < argc)
            {
                priority = atoi(argv[i + 1]);
            }
        }
        else if (std::string(argv[i]) == "-t") {
            // set program runtime
            if (i + 1 < argc)
            {
                run_period = atoi(argv[i + 1]);
            }
        }
        else if (std::string(argv[i]) == "-b") {
            // set scheduling policy to SCHED_BATCH
            policy = SCHED_BATCH;
        }
    }

    //Set scheduler and priority.
    param.sched_priority = priority;
    if (sched_setscheduler(0, policy, &param) == -1) {
        fprintf(stderr,"Error setting scheduler ... are you root?\n");
        exit(1);
    }

    // set up signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1)
        errExit("sigaction");

    sa2.sa_flags = SA_SIGINFO;
    sa2.sa_sigaction = handler2;
    sigemptyset(&sa2.sa_mask);
    if (sigaction(SIG2, &sa2, NULL) == -1)
        errExit("sigaction");

    // Create the timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timer;
    if (timer_create(CLOCKID, &sev, &timer) == -1)
        errExit("timer_create");

    // set timer init offset and period
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1000 / hertz * 1000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    // Create the timer
    sev2.sigev_notify = SIGEV_SIGNAL;
    sev2.sigev_signo = SIGRTMIN+1;
    sev.sigev_value.sival_ptr = &timer2;
    if (timer_create(CLOCKID, &sev2, &timer2) == -1)
        errExit("timer_create2");

    // set timer init offset and period
    its2.it_value.tv_sec = run_period;
    its2.it_value.tv_nsec = 0;
    its2.it_interval.tv_sec = 0;
    its2.it_interval.tv_nsec = 0;


    // start the timer
    if (timer_settime(timer, 0, &its, NULL) == -1)
        errExit("timer_settime");

    if (run_period > 0) {
            // start the timer
        if (timer_settime(timer2, 0, &its2, NULL) == -1)
            errExit("timer2_settime");
    }


    // Main loop
    while (true)
    {
        pause();
    }

    return 0;
}
