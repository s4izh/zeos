# E2

## Mechanisms to enter the system

- [x] Implement the keyboard management
- [x] Implement the `write` system call
    - [x] syscall_handler_sysenter
    - [x] wrapper
    - [x] sys_write
    - [x] bucle sys_write
    - [x] return properly to user mode
- [x] Fast system calls
- [x] Implement the `clock` management
- [x] Implement the `gettime` system call
- [x] Implement the `page fault` exception management
    - [x] quitar el while 1
- [x] Implement `perror`

## Basic process management

- [x] Initialize a free queue.
- [x] Initialize a ready queue.
- [x] Implement the initial processes initialization.
- [x] Implement the task switch function.
- [x] Implement the inner_task_switch function.
- [x] Implement the getpid system call.
- [x] Implement the fork system call.
- [x] Implement process scheduling.
- [x] Implement the exit system call.
- [x] Implement the getstats system call.
