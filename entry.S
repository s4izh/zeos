/*
 */

#include <asm.h>
#include <segment.h>

#include <errno.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %ebx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %edx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
    pushl %gs; \
    pushl %fs; \
    pushl %es; \
    pushl %ds; \
    pushl %eax; \
    pushl %ebp; \
    pushl %edi; \
    pushl %esi; \
    pushl %edx; \
    pushl %ecx; \
    pushl %ebx; \
    movl $__KERNEL_DS, %edx;    \
    movl %edx, %ds;           \
    movl %edx, %es

#define RESTORE_ALL \
    popl %ebx; \
    popl %ecx; \
    popl %edx; \
    popl %esi; \
    popl %edi; \
    popl %ebp; \
    popl %eax; \
    popl %ds; \
    popl %es; \
    popl %fs; \
    popl %gs; \

//  se mueve el valor 0x20 al registro %al
//  el valor 0x20 significa fin de interrupción
// escribe el valor del registro al al puerto 0x20
// que es el puerto de comando del controlador de int
// EOI sirve para decir ya hemos tratado la interrupción
// y podemos ejecutar otra

#define EOI \
   movb $0x20, %al; \
   outb %al, $0x20;

// 32
ENTRY(clock_handler)
   SAVE_ALL
   EOI
   call clock_routine
   RESTORE_ALL
   iret

// 33
ENTRY(keyboard_handler)
    SAVE_ALL
    call keyboard_routine
    EOI
    RESTORE_ALL
    iret

// 14
ENTRY(page_fault_handler2)
    call page_fault_routine2

ENTRY(writeMSR)
    pushl %ebp
    movl %esp, %ebp
    movl 0x8(%ebp), %ecx
    movl 0xC(%ebp), %edx
    movl 0x10(%ebp), %eax
    wrmsr
    movl %ebp, %esp
    pop %ebp
    ret

ENTRY(syscall_handler_sysenter)
    push $__USER_DS;
    push %ebp; // user stack address
    pushfl
    push $__USER_CS
    push 4(%ebp) // user return address
    SAVE_ALL
    cmpl $0, %eax
    jl sysenter_err
    cmpl $MAX_SYSCALL, %eax
    jg sysenter_err
    call *sys_call_table(,%eax,0x04)
    jmp sysenter_fin
sysenter_err:
    movl $-ENOSYS, %eax
sysenter_fin:
    movl %eax, 0x18(%esp)
    RESTORE_ALL
    movl (%esp), %edx // return address
    movl 12(%esp), %ecx // user stack address
    sti               // enable interrupts again
    sysexit

/* void task_switch(union task_union *new) */
ENTRY(task_switch)
    push %ebp
    movl %esp, %ebp
    push %ebx
    push %esi
    push %edi // estos 3 se salvan porque no los salva el compilador
    pushl 8(%ebp) // direccion de retorno de new
    call inner_task_switch
    addl $4, %esp
    popl %edi
    popl %esi 
    popl %ebx
    ret

/* void stack_swap(unsigned long *old_sp, unsigned long new_sp); */
ENTRY(stack_swap)
    movl 4(%esp), %eax
    movl %ebp, (%eax)
    movl 8(%esp), %esp
    popl %ebp
    ret
