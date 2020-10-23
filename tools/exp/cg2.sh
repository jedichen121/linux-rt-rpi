#!/bin/bash

sudo su
mkdir /sys/fs/cgroup/cpu/cg
echo 600000 > /sys/fs/cgroup/cpu/cg/cpu.rt_runtime_us
mkdir /sys/fs/cgroup/cpu/cg/cg2
echo 450000 > /sys/fs/cgroup/cpu/cg/cg2/cpu.rt_runtime_us
#echo 500 > /sys/fs/cgroup/cpu/cg/cg2/cpu.window_us
echo $1 > /sys/fs/cgroup/cpu/cg/cg2/cgroup.procs
exit
