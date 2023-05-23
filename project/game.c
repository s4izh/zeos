#include <game.h>
#include <libc.h>

#define NULL 0

float frames = 0;

struct ball {
  int x;
  int y;
  int x_direction;
  int y_direction;
  float speed; // segundos por movimiento
};

struct ball ball;

float get_fps()
{
  float seconds = (float)gettime()/18.0f;
  return frames/seconds;
}

float get_frames()
{
  return frames;
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
  clear_screen();
  char* buff = "Bienvenido al Fronton!";
  gotoxy(25, 8);
  write(1, buff, strlen(buff));

  buff = "Presiona c para comenzar";
  gotoxy(20, 11);
  write(1, buff, strlen(buff));

  buff = "a jugar al fronton contra ti mismo";
  gotoxy(20, 12);
  write(1, buff, strlen(buff));

  buff = "Presiona ESC para salir";
  gotoxy(25, 15);
  write(1, buff, strlen(buff));

  ++frames;
}

void draw_ball()
{
  gotoxy(ball.x, ball.y);
  char* buff = " ";
  write(1, buff, strlen(buff));

  ball.x += ball.x_direction;
  ball.y += ball.y_direction;

  gotoxy(ball.x, ball.y);
  buff = "O";
  write(1, buff, strlen(buff));
}

void init_game()
{
  clear_screen();
  welcome_screen();

  ball.x = 40;
  ball.y = 10;
  ball.speed = 3.0f;

  ball.x_direction = 1;
  ball.y_direction = 1;

  sleep(10.0f);

  clear_screen();

  /* sleep(10); */
  void *exit_indicator = shmat(1, 284<<12);
  int* exit = (int *)exit_indicator;
  *exit = 0;

  while(*exit == 0) {
    draw_ball(1, 1);
    sleep(ball.speed);
  }

  clear_screen();
}
