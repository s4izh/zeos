# ZeOS

ZeOS is the skeleton of a simple Operating System based on a Linux 2.4
kernel and developed for the intel 80386 architecture. The goal of
this project is to extend its functionalities.

## How to use the flake for building the development environment

Install [nix](https://nixos.org/download.html)
and enable [nix flakes](https://nixos.wiki/wiki/Flakes).

Then on this project's directory run:

```bash
nix develop
```

You will now be inside a shell with `bochs`, `gdb`, `gcc`, etc. available.


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