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

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)


static void
handler(int sig, siginfo_t *si, void *uc)
{
    syscall(400);
}

int main(int argc, char * argv[])
{
    int hertz = 10;        // outerloop frequency, hertz.
    int priority = 30;      // process real-time priority
    int policy = SCHED_FIFO;

    struct sched_param param;
    timer_t timer;
    struct sigevent sev;
    struct sigaction sa;
    struct itimerspec its;

    for (size_t i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-hz")
        {
            if (i + 1 < argc)
            {
                hertz = atof(argv[i + 1]);
            }
        }
        else if (std::string(argv[i]) == "-p") {
            if (i + 1 < argc)
            {
                priority = atoi(argv[i + 1]);
            }
        }
        else if (std::string(argv[i]) == "-b") {
            policy = SCHED_BATCH;
        }
    }

    //Set scheduler and priority.
    param.sched_priority = priority;
    if (sched_setscheduler(0, policy, &param) == -1) {
        fprintf(stderr,"error setting scheduler ... are you root?\n");
        exit(1);
    }

    std::cout << " [Arg] run at: " << hertz << " hertz\n";

    // set up signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1)
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


    // start the timer
    if (timer_settime(timer, 0, &its, NULL) == -1)
        errExit("timer_settime");
    
    std::cout << "pid is: " << getpid() << "\n";
    // Main loop
    while (true)
    {
        pause();
    }

    return 0;
}
