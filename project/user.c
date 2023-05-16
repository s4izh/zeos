#include <libc.h>

#define NULL 0

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */


  // test control ----------------

  int pagefault_test = 0;
  int setcolor_test = 1; // milestone 3
  int gotoxy_test = 1; // milestone 3
  int shmat_test = 1; // milestone 1 y 2
  int read_test = 1; // milestone 1 y 2

  // pagefault_test -------------

  if (pagefault_test) {
    int *a = 0;
    *a = 0;
  }

  // setcolor_test -------------

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
      write(1, "hola blanco ", 12);
    }
    if (set_color(0, 7) != -1) {
      write(1, "hola negro ", 11);
    }
    if (set_color(2, 0) != -1) {
      write(1, "hola verde ", 11);
    }
  }

  // gotoxy_test ----------------

  if (gotoxy_test) {
    gotoxy(0, 0);
    char *buff = "hola 0,0";
    write(1, buff, strlen(buff));
  }

  // shmat_test -----------------

  if (shmat_test) {
    void *shared_mem;

    // Test invalid id
    shared_mem = shmat(-1, NULL);
    if (shared_mem == -1) {
      char* buff = "\ninvalid id test passed, perror: ";
      write(1, buff, strlen(buff));
      perror();
    } else {
      char* buff = "\ninvalid id test failed, perror: ";
      write(1, buff, strlen(buff));
      perror();
    }

    // Test NULL addr
    shared_mem = shmat(1, NULL);
    if (shared_mem == -1) {
      char* buff = "\nNULL addr test failed\n";
      write(1, buff, strlen(buff));
      perror();
    } else {
      // Test write in NULL addr
      int* data = (int*)shared_mem;
      *data = 42;
      if (*data == 42) {
        char* buff = "\nNULL addr test passed\n";
        write(1, buff, strlen(buff));
      }
      shared_mem = shmat(1, 342>>12);
      int* data2 = (int*)shared_mem;
      if (*data == *data2) {
        char* buff = "both pages mapped to the same frame passed\n";
        write(1, buff, strlen(buff));
      }
    }

    shared_mem = shmat(3, NULL);
    int* data = (int*)shared_mem;
    *data = 42;

    void* shared_mem2 = shmat(3, NULL);

    if (shmdt(shared_mem) < 0) {
      char* buff = "error shmdt, perror: ";
      write(1, buff, strlen(buff));
      perror();
    }

    shmrm(3);

    shmdt(shared_mem);

    int* data2 = (int*)shared_mem2;
    if (42 == *data2) {
      char* buff = "\nshmdt funciona\n";
      write(1, buff, strlen(buff));
    }

    shmdt(shared_mem2);

    shared_mem2 = shmat(3, shared_mem2);

    int* data3 = (int*)shared_mem2;
    if (0 == *data3) {
      char* buff = "shmrm funciona\n";
      write(1, buff, strlen(buff));
    }

    int pid = fork();

    switch (pid) {
    case -1:
      char* buff = "fork failed\n";
      write(1, buff, strlen(buff));
      break;
    case 0:
      shared_mem = shmat(3, NULL);
      int* data5 = (int*)shared_mem;
      *data5 = 42;
      shmdt(shared_mem);
      break;
    default:
      shared_mem = shmat(3, 320);
      int* data6 = (int*)shared_mem;
      *data6 = 42;
      shmdt(shared_mem);
      break;
    }


    /* int res = shmdt(shared_mem); */
    /* if (res == 0) { */
    /*   int *i = (int*)shared_mem; */
    /*   ++*i; */
    /*   char test[10]; */
    /*   itoa(*i, test); */
    /*   write(1, test, strlen(test)); */
    /* } */

    // Test 
  }

  // read_test ------------------

  if (read_test) {
    while (1) {
      char buff[4];
      int c = read(buff, 4);
      if (c != -1) {
        write(1, buff, c);
      }
    }
  }

  while(1) { }
}
