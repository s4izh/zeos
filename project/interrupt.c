/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>

#include <mm.h>

#include <sched.h>

#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;

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

int zeos_ticks = 0;

void clock_routine()
{
  zeos_show_clock();
  zeos_ticks ++;
  
  schedule();
}

void keyboard_routine()
{
  unsigned char c = inb(0x60);
  
  /* if (c&0x80) printc_xy(0, 0, char_map[c&0x7f]); */
  if (c&0x80) write_buff(char_map[c&0x7f]);
}


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

void clock_handler();
void keyboard_handler();
void system_call_handler();
void page_fault_handler2();

void setMSR(unsigned long msr_number, unsigned long high, unsigned long low);

void setSysenter()
{
  setMSR(0x174, 0, __KERNEL_CS);
  setMSR(0x175, 0, INITIAL_ESP);
  setMSR(0x176, 0, (unsigned long)system_call_handler);
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

  setSysenter();

  set_idt_reg(&idtR);
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
  unsigned pag = eip >> 12;

  if (pag >= PAG_LOG_INIT_DATA && pag < PAG_LOG_INIT_DATA + NUM_PAG_DATA) {
    page_table_entry *process_PT = get_PT(current());
    int frame = get_frame(process_PT, pag);

    if (phys_mem[frame] == 1) {
      set_ss_pag(process_PT, pag, frame);
      return;
    }

    if (phys_mem[frame] > 1) {
      int new_frame = alloc_frame();
      if (new_frame != -1) {
        void *addr = get_free_addr(process_PT);
        set_ss_pag(process_PT, (unsigned)addr >> 12, new_frame);
        copy_data((void*)(pag << 12), addr, PAGE_SIZE);

        del_ss_pag(process_PT, pag);
        del_ss_pag(process_PT, (unsigned)addr >> 12);

        --phys_mem[frame];

        set_ss_pag(process_PT, pag, new_frame);
        set_cr3(get_DIR(current()));
        return;
      }
    }
  }

  char buff[8];
  int_to_hex(eip, buff);
  printk("Process generates a PAGE FAULT expection at EIP: 0x");
  printk(buff);
  while(1);
}

