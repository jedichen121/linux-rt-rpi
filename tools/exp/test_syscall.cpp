/*
 * Test the stephen syscall (#329)
 */
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

/*
 * Put your syscall number here.
 */
#define SYS_number 400
#define SYS_cpu_block_num 400
#define SYS_cpu_unblock_num 401

int main(int argc, char **argv)
{
  long res = syscall(SYS_number);
  if (res == 0)
    printf("System call success\n");

  return res;
}
