#!/bin/bash

sudo su
mkdir /sys/fs/cgroup/cpu/cg
echo 500000 > /sys/fs/cgroup/cpu/cg/cpu.rt_runtime_us
mkdir /sys/fs/cgroup/cpu/cg/cg1
echo 100000 > /sys/fs/cgroup/cpu/cg/cg1/cpu.rt_runtime_us
echo 80000 > /sys/fs/cgroup/cpu/cg/cg1/cpu.window_us
echo $1 > /sys/fs/cgroup/cpu/cg/cg1/cgroup.procs
exit
