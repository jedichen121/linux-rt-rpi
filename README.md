# Linux kernel for blocking RT task from execution for certain periods.

## Major design

Implemented a new system call `cpu_block()` to block all task from running for specified length except itself and tasks with rt priority high than 50 (kernel threads). It is implemented using a hrtimer that has the period specified in the calling process's cgroup `cpu.window_us`. The syscall has a syscall number of 400 and no input parameters. You can invoke it using `syscall(400)`.


Add a parameter `window_us` for cgroup to specify the blocking time in microseconds. To set the parameter

```
echo time_in_us > /sys/fs/cgroup/cpu/cg/cg1/cpu.window_us
```

* _note_: The blocking function will not work unless this parameter is set. 

Some cgroup setup scripts and test programs are in the folder `tools/exp/`.

__Important__: If the window is too long compared with the period of the invoking task, the whole system might become unresponsive. 


Linux kernel
============

There are several guides for kernel developers and users. These guides can
be rendered in a number of formats, like HTML and PDF. Please read
Documentation/admin-guide/README.rst first.

In order to build the documentation, use ``make htmldocs`` or
``make pdfdocs``.  The formatted documentation can also be read online at:

    https://www.kernel.org/doc/html/latest/

There are various text files in the Documentation/ subdirectory,
several of them using the Restructured Text markup notation.
See Documentation/00-INDEX for a list of what is contained in each file.

Please read the Documentation/process/changes.rst file, as it contains the
requirements for building and running the kernel, and information about
the problems which may result by upgrading your kernel.
