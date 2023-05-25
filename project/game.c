#include <game.h>
#include <libc.h>

#define NULL 0

#define MAX_LEVEL 15
#define MAX_TRIES 5

float frames = 0.0f;

float get_fps()
{
  float seconds = (float)gettime()/18.0f;
  return frames/seconds;
}


volatile char* char_read = NULL;
/* unsigned int* magic_number; */

char *words[15] = {
  // facilito
  "gestor",
  "wrapper",
  "syscall",
  "kernel",
  "fork",

  // medio
  "thread",
  "tlb",
  "pagefault",
  "quantum",
  "idle",

  // dificil
  "iofin",
  "iorb",
  "scheduler",
  "cr3",
  "pajuelo",
};

char *secret_word;

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
  buff = "Varios alumnos han sido secuestrados";
  gotoxy(5, 11);
  write(1, buff, strlen(buff));

  set_color(7, 0);
  buff = "por el malvado profesor Baka Baka...";
  gotoxy(5, 12);
  write(1, buff, strlen(buff));

  set_color(7, 0);
  buff = "Ayudalos a escapar adivinando las siguuientes";
  gotoxy(5, 13);
  write(1, buff, strlen(buff));

  buff = "palabras, creemos que tienen que ver con SOA ";
  gotoxy(5, 14);
  write(1, buff, strlen(buff));

  /* buff = "(el vocabulario de SOA)"; */
  /* gotoxy(5, 15); */
  /* write(1, buff, strlen(buff)); */


  set_color(4, 0);
  buff = "Solo tu puedes salvarlos";
  gotoxy(25, 19);
  write(1, buff, strlen(buff));

  set_color(2, 0);
  buff = "Pulsa c para empezar a jugar";
  gotoxy(25, 20);
  write(1, buff, strlen(buff));

  ++frames;
}

void marco() {
  /* set_color(0, 1); */
  gotoxy(0, 0);
  write(1, "                                                                                ", 80);
  gotoxy(0, 24);
  write(1, "                                                                                ", 80);
}

void initial_screen() 
{
  clear_screen();
  welcome_screen();
  set_color(0, 1);
  marco();
  set_color(15, 0);
  for (int i = 0; i < 7; ++i)
    draw_hangman(i);
}

void init_reader() 
{
  while (1) {
    char buff[1];
    if (read(buff, 1) > 0) {
      *char_read = buff[0];
      /* write(1, buff, 1); */
    }
  }
}

void game_loop()
{
  initial_screen();

  char c = *char_read;
  int game_started = 0;
  int score = 0;

  int level = 0;

  int letter_count;

  int guesses [50];
  int failed_tries = 0;

  int last_round_over = 0;

  while (1) {
    if (!game_started) {
      if (*char_read == 'c' || last_round_over) {

        game_started = 1;
        last_round_over = 0;
        clear_screen();
        set_color(15, 0);

        if (level == MAX_LEVEL) {
          clear_screen();
          set_color(0, 2);
          marco();
          set_color(2, 0);
          char* buff = "Felicidades ";
          gotoxy(10, 8);
          write(1, buff, strlen(buff));
          buff = "los alumnos han sido liberados y por suerte";
          gotoxy(10, 9);
          write(1, buff, strlen(buff));
          buff = "no los volveras a ver en SOA";
          gotoxy(10, 10);
          write(1, buff, strlen(buff));
          sleep(1);
          char* buff4 = "Presiona c para volver a jugar";
          gotoxy(10, 12);
          write(1, buff4, strlen(buff4));
          level = 0;
          score = 0;
          game_started = 0;
          last_round_over = 0;
          continue;
        }

        clear_screen();

        set_color(0, 1);
        marco();
        set_color(15, 0);

        if (level >= 0 && level < 5) {
          gotoxy(30, 4);
          set_color(2, 0);
          char* buff = "Nivel facil";
          write(1, buff, strlen(buff));
        }
        if (level >= 5 && level < 10) {
          gotoxy(30, 4);
          set_color(6, 0);
          char* buff = "Nivel medio";
          write(1, buff, strlen(buff));
        }
        if (level >= 10 && level < 15) {
          gotoxy(30, 4);
          set_color(4, 0);
          char* buff = "Nivel dificil";
          write(1, buff, strlen(buff));
        }
        gotoxy(25, 5);
        char* bufff = "Palabras resueltas: ";
        write(1, bufff, strlen(bufff));
        char buff3[2];
        itoa(level, buff3);
        write(1, buff3, strlen(buff3));
        bufff = " / 15";
        write(1, bufff, strlen(bufff));

        set_color(15, 0);

        draw_hangman(0);

        secret_word = words[level];
        /* write(1, secret_word, strlen(secret_word)); */ 
        letter_count = strlen(secret_word);

        gotoxy(2, 20);
        char *buff2= "Intentos: ";
        write(1, buff2, strlen(buff2));
        gotoxy(10, 10);
        for (int i = 0; i < strlen(secret_word); ++i) {
          write(1, "_", 1);
        }
      } 
      c = *char_read;
      for (int i = 0; i < 50; ++i) {
        guesses[i] = 0;
      }
    }
    

    /* la buena chapuza */

    /* *magic_number++; */
    /* if (magic_number == 0xFFFFFFFF) magic_number = 0; */


    else if (c != *char_read) {
      if (guesses[*char_read - 'a'] == 1) {
        c = *char_read;
        continue;
      }
      int present = 0;
      for (int i = 0; i < strlen(secret_word); ++i) {
        if (secret_word[i] == *char_read) {
          gotoxy(10 + i, 10);
          write(1, char_read, 1);
          present = 1;
          --letter_count;
          if (letter_count == 0) {
            game_started = 0;
            last_round_over = 1;
            failed_tries = 0;
            ++level;
          }
        }
      }
      if (!present) {
        ++failed_tries;
        set_color(15, 0);
        draw_hangman(failed_tries);
        gotoxy(11 + failed_tries, 20);

        set_color(12, 0);
        write(1, char_read, 1);
        set_color(15, 0);

        if (failed_tries == MAX_TRIES) {
          clear_screen();
          set_color(0, 4);
          marco();
          set_color(12, 0);
          for (int i = 0; i < 7; ++i)
            draw_hangman(i);

          set_color(2, 0);
          char* buff = "Has salvado a ";
          gotoxy(10, 8);
          write(1, buff, strlen(buff));

          char n[2];
          itoa(level, n);
          write(1, n, 1);

          buff = " / 15 inocentes";
          write(1, buff, strlen(buff));

          set_color(4, 0);
          gotoxy(10, 10);
          buff = "este ultimo no ha tenido tanta suerte...";
          write(1, buff, strlen(buff));

          /* set_color(3, 0); */
          gotoxy(10, 12);
          buff = "presiona c para volver a intentarlo";
          write(1, buff, strlen(buff));
          level = 0;
          score = 0;
          game_started = 0;
          last_round_over = 0;
          failed_tries = 0;
          if (*char_read == 'c') *char_read = 'd'; // truco sucio para que no empiece la partida
          continue;
        }
      }
      guesses[*char_read - 'a'] = 1;
      c = *char_read;
    }
  }
}

void init_game()
{
  void* shared_mem = shmat(1, NULL);
  
  char_read = (char*)((unsigned long) shared_mem);

  /* magic_number = (int*)(((unsigned long) shared_mem) + 8); */
  /* *magic_number = 0; */

  int pid = fork();

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
