/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>

#include <libc.h>

#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;

int zeos_ticks = 0;

void clock_handler();
void keyboard_handler();
void page_fault_handler2();
void syscall_handler_sysenter();
void writeMSR();

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','¡','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','ñ',
  '\0','º','\0','ç','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}


void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;

  set_handlers();

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */

  setInterruptHandler(14, page_fault_handler2, 0);
  setInterruptHandler(32, clock_handler, 0);
  setInterruptHandler(33, keyboard_handler, 0);

  writeMSR(0x174, 0, __KERNEL_CS);
  writeMSR(0x175, 0, INITIAL_ESP);
  writeMSR(0x176, 0, syscall_handler_sysenter);

  set_idt_reg(&idtR);
}

void clock_routine(void){
  zeos_ticks++;
  zeos_show_clock();
  schedule();
}

void keyboard_routine(void){
    // input del keyboard data register 0x60
    unsigned char input = inb(0x60);
    // bit de más peso
    unsigned char state = input >> 7;

    if (state == 0) {
        char trad = char_map[input & 0x7F];
        if (trad) printc_xy(60, 10, trad);
        else printc_xy(60, 10, 'C');
    }
}

void int_to_hex(unsigned int num, char *hex_string) {
    const char *hex_digits = "0123456789abcdef";
    int i = 0;

    while (num > 0) {
        int digit = num % 16;
        hex_string[i++] = hex_digits[digit];
        num /= 16;
    }
    while (i < sizeof(int) * 2) {
        hex_string[i++] = '0';
    }
    int len = i;
    for (i = 0; i < len / 2; i++) {
        char tmp = hex_string[i];
        hex_string[i] = hex_string[len - i - 1];
        hex_string[len - i - 1] = tmp;
    }
    hex_string[len] = '\0';
}

void page_fault_routine2(unsigned int error, unsigned int eip) {
    char buff[8];
    int_to_hex(eip, buff);
    printk("Process generates a PAGE FAULT expection at EIP: 0x");
    printk(buff);
    itoa(error, buff);
    printk("\nError: ");
    printk(buff);
    while(1);
}
