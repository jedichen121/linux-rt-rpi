#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

static volatile int keepRunning = 1;

void sigintHandler(int dummy) {
    keepRunning = 0;
}


void test_little(void)
{
  int i,j;

  for(i = 0; i < 30000000; i++) 
    j=i; 
}

void test_medium(void)
{
  int i,j;

  for(i = 0; i < 60000000; i++) 
    j=i; 
}

void test_high(void)
{
  int i,j;

  for(i = 0; i < 90000000; i++) 
    j=i; 
}

void test_hi(void)
{
  int i,j;


  for(i = 0; i < 120000000; i++) 
    j=i; 
}

int main(void)
{
  int i, pid, result;
  cpu_set_t mask;
  struct sched_param param;

  //Set scheduler and priority.
  param.sched_priority = 10;
//  printf("before sched_setscheduler\n");
  if (sched_setscheduler(0, 1, &param) == -1) {
        fprintf(stderr,"error setting scheduler ... are you root?\n");
        exit(1);
  }
//  printf("after sched_setscheduler\n");
//Set CPU affinity.
//   CPU_ZERO(&mask);
//   CPU_SET(0, &mask);
//   if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
//   {
//     exit(EXIT_FAILURE);
//   }

  int policy = sched_getscheduler(0);
  printf("The current scheduling policy is: %d\n",policy);
  signal(SIGINT, sigintHandler);

  int count = 0;
  printf("pid = %d\n", getpid());
  while (keepRunning) {
    test_little();
//    printf("thread %d running %d\n", getpid(), count);
    count++;
    if (count == 10)
      count = 0;
  }

  return 0;
}
