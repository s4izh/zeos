#include <libc.h>

#define NULL 0

char buff[24];
int pid;

void test_shmat()
{
  void* shared_mem = shmat(1, NULL);
  void* shared_mem2 = shmat(1, 342<<12);

  int* data = (int *)shared_mem;
  *data = 10;

  int* data2 = (int *)shared_mem2;

  if (*data == *data2) {
    char* buff = "\ntest_shmat(): passed\n";
    write(1, buff, strlen(buff));
  }
  else {
    char* buff = "\ntest_shmat(): failed\n";
    write(1, buff, strlen(buff));
  }
  shmdt(shared_mem);
  shmdt(shared_mem2);
}

void test_shmrm()
{
  void* shared_mem = shmat(2, NULL);
  void* shared_mem2 = shmat(2, 342<<12);

  int* data = (int *)shared_mem;
  *data = 10;

  shmrm(2);
  shmdt(shared_mem);
  shmdt(shared_mem2);
  shared_mem2 = shmat(2, 340<<12);
  
  int* data2 = (int *)shared_mem2;

  if (0 == *data2) {
    char* buff = "test_shmrm(): passed\n";
    write(1, buff, strlen(buff));
  }
  else {
    char* buff = "test_shmrm(): failed\n";
    write(1, buff, strlen(buff));
  }
  shmdt(shared_mem2);
}

void test_shmat_fork()
{
  void* shared_mem = shmat(3, NULL);
  void* shared_mem2 = shmat(3, 342<<12);

  int* data = (int *)shared_mem;
  *data = 10;

  int pid = fork();

  if (pid == 0) {
    int* data2 = (int *)shared_mem2;

    if (*data == *data2) {
      char* buff = "test_shmat_fork(): passed\n";
      write(1, buff, strlen(buff));
    }
    else {
      char* buff = "test_shmat_fork(): failed\n";
      write(1, buff, strlen(buff));
    }
    exit();
  }
  shmdt(shared_mem);
  shmdt(shared_mem2);
}

void test_shmrm_fork()
{
  void* shared_mem = shmat(4, NULL);
  void* shared_mem2 = shmat(4, 342<<12);

  int* data = (int *)shared_mem;
  *data = 10;

  int pid = fork();

  shmrm(4);
  shmdt(shared_mem);
  shmdt(shared_mem2);
  shared_mem2 = shmat(4, 342<<12);

  int* data2 = (int *)shared_mem2;

  if (pid == 0) {
    if (0 == *data2) {
      char* buff = "test_shmrm_fork(): passed\n";
      write(1, buff, strlen(buff));
    }
    else {
      char* buff = "test_shmrm_fork(): failed\n";
      write(1, buff, strlen(buff));
    }
  }
  shmdt(shared_mem2);
}

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
    test_shmat();
    test_shmrm();
    test_shmat_fork();
    test_shmrm_fork();
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
