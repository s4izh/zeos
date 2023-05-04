#include <libc.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  int pagefault_test = 0;
  int read_test = 0;
  int setcolor_test = 1;
  int gotoxy_test = 1;

  if (pagefault_test) {
    int *a = 0;
    *a = 0;
  }

  if (read_test) {
    while (1) {
      char buff[4];
      int c = read(buff, 4);
      if (c != -1) {
        write(1, buff, c);
      }
    }
  }

  if (setcolor_test) {
    if (set_color(3, 0) != -1) {
      write(1, "hola cyan ", 10);
    }
    if (set_color(4, 0) != -1) {
      write(1, "hola rojo ", 10);
    }
    if (set_color(5, 0) != -1) {
      write(1, "hola rosa ", 10);
    }
    if (set_color(15, 0) != -1) {
      write(1, "hola blanco ", 11);
    }
  }

  if (gotoxy_test) {
    gotoxy(-1, 0);
    char *buff = "hola 0,0";
    write(1, buff, strlen(buff));
  }

  while(1) { }
}
