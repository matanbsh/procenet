# ProceNet – Linux Kernel Extension
A kernel module that models social interactions between processes using custom system calls and a modified scheduler.

## Features
- Adds new system calls: `invite`, `respond`, `get_invitations`, `get_info`
- Each process maintains a profile with social links using `task_struct`
- Scheduler prioritizes processes based on number of received invitations
- Kernel-level implementation using syscall table and in-kernel linked lists

## Technologies
- C
- Linux Kernel 2.4
- Kernel structs and memory
- System call table editing
- Custom scheduling policy

## How to Compile & Load
```bash
make
sudo insmod procenet.ko
