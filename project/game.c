#include <game.h>
#include <libc.h>

#define NULL 0

#define MAX_TRIES 6
#define MAX_WORD_LENGTH 20

int tries = 0;

float frames = 0;

int* exit_flag;
char* char_read = NULL;


char *words[20] = {
  "gestor",
  "wrapper",
  "syscall",
  "scheduler",
  "kernel",
  "trhead",
  "iorb",
  "iofin",
};

void draw_hangman(int tries) {
  char *buff = "   +---+";
  gotoxy(57, 7);
  write(1, buff, strlen(buff));

  buff = "   |   |";
  gotoxy(57, 8);
  write(1, buff, strlen(buff));

  if (tries >= 1) {
    buff = "   O   |";
    gotoxy(57, 9);
    write(1, buff, strlen(buff));
  } 
  else {
    buff = "       |";
    gotoxy(57, 9);
    write(1, buff, strlen(buff));
  }

  if (tries == 2) {
    buff = "  /    |";
    gotoxy(57, 10);
    write(1, buff, strlen(buff));
  } else if (tries == 3) {
    buff = "  /|   |";
    gotoxy(57, 10);
    write(1, buff, strlen(buff));
  } else if (tries >= 4) {
    buff = "  /|\\  |";
    gotoxy(57, 10);
    write(1, buff, strlen(buff));
  } else {
    buff = "       |";
    gotoxy(57, 10);
    write(1, buff, strlen(buff));
  }

  if (tries == 5) {
    gotoxy(57, 11);
    buff = "  /    |";
    write(1, buff, strlen(buff));
  } else if (tries >= 6) {
    gotoxy(57, 11);
    buff ="  / \\  |";
    write(1, buff, strlen(buff));
  } else {
    gotoxy(57, 11);
    buff = "       |";
    write(1, buff, strlen(buff));
  }

  gotoxy(57, 12);
  buff = "       |";
  write(1, buff, strlen(buff));

  gotoxy(57, 13);
  buff = "=========";
  write(1, buff, strlen(buff));
}

void clear_hangman() {
  void * buff = "             ";
  for (int i = 0; i < 7; i++) {
    gotoxy(57, 7 + i);
    write(1, buff, strlen(buff));
  }

}

float get_fps()
{
  float seconds = (float)gettime()/18.0f;
  return frames/seconds;
}

void draw_fps()
{
  int fps = get_fps() * 100;
  char buff[4];
  itoa(fps, buff);
  /* gotoxy(0, 60); */
  /* write(1, buff, strlen(buff)); */
  /* write(1, " .0 fps", 7); */
}

void clear_screen()
{
  gotoxy(0, 0);
  char* buff = "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ";
  // de esta manera se limpia toda la pantalla con una sola llamada a sistema
  write(1, buff, strlen(buff));
}

void welcome_screen()
{
  /* clear_screen(); */
  set_color(15, 0);
  char* buff = "Bienvenido al Ahorcado de SOA!";
  gotoxy(10, 8);
  write(1, buff, strlen(buff));

  set_color(7, 0);
  buff = "Presiona c para comenzar";
  gotoxy(5, 11);
  write(1, buff, strlen(buff));

  buff = "a jugar al ahorcado con las peores palabras";
  gotoxy(5, 12);
  write(1, buff, strlen(buff));

  buff = "jamas pensadas (el vocabulario de SOA)";
  gotoxy(5, 13);
  write(1, buff, strlen(buff));

  set_color(4, 0);
  buff = "Presiona ESC para salir";
  gotoxy(25, 20);
  write(1, buff, strlen(buff));

  ++frames;

  /* draw_fps(); */
}

void marco() {
  set_color(0, 1);
  gotoxy(0, 0);
  write(1, "                                                                                ", 80);
  gotoxy(0, 24);
  write(1, "                                                                                ", 80);
}

void initial_screen() 
{
  clear_screen();
  welcome_screen();
  marco();
  set_color(15, 0);
  for (int i = 0; i < 7; ++i)
    draw_hangman(i);
}

void init_reader() 
{
  while (1) {
    char buff[1];
    int c = read(buff, 1);
    if (c != -1) {
      *char_read = buff[0];
      *char_read = 'a';
      write(1, buff, c);
    }
  }
  /* int* exit_flag; */
  /* char* char_read; */
}

void game_loop(){
  initial_screen();

  char c = *char_read;

  /* *char_read = 'a'; */

  /* while (1) { */
  /* } */

  while (1)
  {
    if (*char_read != c) {
      clear_hangman();
      draw_hangman(0);
      c = *char_read;
    }
  }

  /* return; */
};

void init_game()
{
  int pid = fork();

  void *shared_mem = shmat(1, NULL);
  exit_flag = (int*)shared_mem;
  *exit_flag = 0;

  char_read = (char*)(shared_mem + 4);

  switch (pid)
  {
    case -1:
      clear_screen();
      set_color(12, 0);
      gotoxy(25, 10);
      char *buff = "Error en el fork del juego";
      write(1, buff, strlen(buff));
      exit();
      break;

    case 0:
      init_reader();
      break;

    default:
      /* while (1) { */
        game_loop();
      /* } */
  }

  /* set_color(15, 0); */
  /* clear_hangman(); */
  /* draw_hangman(0); */

  /* sleep(5); */
}
