#include <asm.h>

syscall_sysenter:
    // ecx y edx se guardan porque son modificados
    // después de restaurar el contexto software
    // y antes de hacer el sysexit.

    // sysexit necesita:
    // dirección de retorno en edx
    // dirección del stack de usuario en ecx
    pushl %ecx
    pushl %edx
    pushl $sysenter_return
    pushl %ebp
    movl %esp, %ebp
    sysenter
sysenter_return:
    popl %ebp // pop a %ebp
    popl %edx // pop de $sysenter_return
    popl %edx // recuperamos %edx
    popl %ecx // recuperamos %ecx
    ret

handle_error:
    neg %eax
    mov %eax, errno
    mov $-1, %eax
    popl %ebp // pop del ebp de la función que ha llamado al wrapper
    ret 

/* int write(int fd, char *buffer, int size) */
ENTRY(write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    movl $4, %eax
    movl 0x8(%ebp), %ebx // fd
    movl 0xC(%ebp), %ecx // buffer
    movl 0x10(%ebp), %edx // size
    // se puede hacer sin el call para ahorrarnos alguna instrucción
    // pero así agrupamos todo el código para preparar la syscall
    call syscall_sysenter
    popl %ebx
    cmpl $0, %eax
    jl handle_error
    popl %ebp // pop del ebp de la función que ha llamado al wrapper
    ret

/* int gettime() */
ENTRY(gettime)
    pushl %ebp
    movl %esp, %ebp
    movl $10, %eax
    call syscall_sysenter
    popl %ebp // pop del ebp de la función que ha llamado al wrapper
    // esta llamada no devuelve error por lo tanto no
    // hace falta que lo gestionemos
    ret

/* int getpid() */
ENTRY(getpid)
    pushl %ebp
    movl %esp, %ebp
    movl $20, %eax
    call syscall_sysenter
    popl %ebp
    ret

/* int fork() */
ENTRY(fork)
    pushl %ebp
    movl %esp, %ebp
    movl $2, %eax
    call syscall_sysenter
    cmpl $0, %eax
    jl handle_error
    popl %ebp
    ret

/* void exit() */
ENTRY(exit)
    pushl %ebp
    movl %esp, %ebp
    movl $1, %eax
    call syscall_sysenter
    popl %ebp
    ret
