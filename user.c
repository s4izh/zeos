#include <libc.h>

char buff[24];

int pid;

void fork_test() {
  char *buff;
  buff = "\n\nFORK TEST\n";
  if (write(1, buff, strlen(buff)) == -1) perror();
  buff = "Creando hijo\n";
  if (write(1, buff, strlen(buff)) == -1) perror();

  int pid = fork();
  char pidbuff[16];
  itoa(pid, pidbuff);
  /* if (write(1, pidbuff, strlen(pidbuff)) == -1) perror(); */
  char ownpid[16];

  char *testing_data = "fragmento de datos del padre copiado en el hijo\n";

  switch(pid) {
  case -1:
    perror();
    break;
  case 0:
    buff = "\nHIJO: getpid == ";
    if (write(1, buff, strlen(buff)) == -1) perror();
    itoa(getpid(), ownpid);
    if (write(1, ownpid, strlen(ownpid)) == -1) perror();
    buff = "\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    if (write(1, testing_data, strlen(testing_data)) == -1) perror();
    break;
  default:
    buff = "PADRE: getpid == ";
    if (write(1, buff, strlen(buff)) == -1) perror();
    itoa(getpid(), ownpid);
    if (write(1, ownpid, strlen(ownpid)) == -1) perror();

    buff = " -- pid de mi hijo == ";
    if (write(1, buff, strlen(buff)) == -1) perror();
    if (write(1, pidbuff, strlen(pidbuff)) == -1) perror();
    buff = "\n";
    if (write(1, buff, strlen(buff)) == -1) perror();



    break;
  }

  buff = "INFO: El codigo siguiente lo tendrian que ejecutar padre e hijo\n";
  if (write(1, buff, strlen(buff)) == -1) perror();

  buff = "Saludos desde getpid == ";
  if (write(1, buff, strlen(buff)) == -1) perror();

  itoa(getpid(), ownpid);
  if (write(1, ownpid, strlen(ownpid)) == -1) perror();

  buff = "\n";
  if (write(1, buff, strlen(buff)) == -1) perror();
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  char *buff;
  buff = "\nModificar user.c para activar/desactivar tests\n";
  if (write(1, buff, strlen(buff)) == -1) perror();

  /* --------------- SETUP -------------------------------- */
  /* -- poner a 1 los tests que se quieran hacer ---------- */

  int test_write = 0;
  int test_gettime = 0; // probar individualmente porque ocupa mucha pantalla
  int test_pagefault = 0; // desactivar para poder probar el teclado y reloj
  int test_getpid = 1;
  int test_fork = 1;

  /* --------------- WRITE TEST ------------------- */

  if (test_write) {
    buff = "\nWRITE TEST\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    buff = "[write1] hola alex\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    buff = "[write2] adios alex\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    buff = "[write3] ahora forzaremos un error EFAULT en el write4, se imprimira errno\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    // provocar error
    if (write(1, 0x0, strlen(buff)) == -1) perror();
    // 5000 carácteres
    buff = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    int byteswritten = write(1, buff, strlen(buff));
    char bw[16];
    itoa(byteswritten, bw);
    if (write(1, bw, strlen(bw)) == -1) perror();
  }

  /* --------------- GETTIME TEST ------------------ */

  // desactivado por defecto, ocupa mucha pantalla

  if (test_gettime) {
    buff = "\n\nGETTIME TEST\n";
    if (write(1, buff, strlen(buff)) == -1) perror();

    int i = 0;
    char time[16];
    char *salto = "\n";

    // probamos bastantes iteraciones para que se vea
    // que va creciendo
    while(i < 10000) {
      if (write(1, salto, strlen(salto)) == -1) perror();
      itoa(gettime(), time);
      if (write(1, time, strlen(time)) == -1) perror();
      ++i;
    }
    buff = "\n\nGETTIME TEST terminado\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
  }

  /* --------------- PAGEFAULT TEST ---------------- */

  // desactivado por defecto porque bloquea

  if (test_pagefault) {
    buff = "\n\nPAGEFAULT TEST\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    char *p = 0;
    *p = 'x';
  }

  /* --------------- GETPID TEST ---------------- */

  if (test_getpid) {
    buff = "\n\nGETPID TEST\n";
    if (write(1, buff, strlen(buff)) == -1) perror();
    int pid = getpid();
    itoa(pid, buff);
    if (write(1, buff, strlen(buff)) == -1) perror();
  }

  /* --------------- FORK TEST ---------------- */

  if (test_fork) {
    fork_test();
    fork_test();
    /* fork_test(); */
    /* char ir[16]; */
    /* for(int i =0; i<10; ++i){ */
    /*   int pid = fork(); */
    /*   itoa(pid,ir); */
    /*   write(1,ir,strlen(ir)); */
    /* } */

    /* buff = "\nhijo 2\n"; */
    /* if (write(1, buff, strlen(buff)) == -1) perror(); */
    /* int pid2 = fork(); */
    /* itoa(pid2, buff); */
    /* if (write(1, buff, strlen(buff)) == -1) perror(); */
  }

  /* ----------------------------------------------- */

  while (1) { };
}
