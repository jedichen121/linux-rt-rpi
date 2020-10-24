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


  int policy = sched_getscheduler(0);
  printf("The current scheduling policy is: %d\n",policy);
  signal(SIGINT, sigintHandler);

  int count = 0;
  printf("pid = %d\n", getpid());
  while (keepRunning) {
    test_hi();
//    printf("thread %d running %d\n", getpid(), count);
    count++;
    if (count == 10)
      count = 0;
  }

  return 0;
}
