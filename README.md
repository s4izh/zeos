# ZeOS

ZeOS is the skeleton of a simple Operating System based on a Linux 2.4
kernel and developed for the intel 80386 architecture. The goal of
this project is to extend its functionalities.

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

- [x] Añadido a `task_struct` el `anchor`
- [x] Inicialización `freeQueue` y `readyQueue`
