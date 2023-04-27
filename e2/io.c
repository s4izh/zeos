/*
 * io.c - 
 */

#include <io.h>

#include <types.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS    25

Byte x, y=19;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
  return v;
}

void printc(char c)
{
     __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
    x = 0;
    y=(y+1)%NUM_ROWS;
  }
  else
  {
    // los 3 bits de menos peso del byte de la izquierda son rgb
    // el byte de la derecha es el char a escribir
    Word ch = (Word) (c & 0x00FF) | 0x0200;
    Word *screen = (Word *)0xb8000;
    screen[(y * NUM_COLUMNS + x)] = ch;
    if (++x >= NUM_COLUMNS)
    {
      x = 0;
      y=(y+1)%NUM_ROWS;
    }
  }
}

/* para imprimir un carácter de un color */
void printc_color(char c, char color)
{
     __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
    x = 0;
    y=(y+1)%NUM_ROWS;
  }
  else
  {
    /* los 3 bits de menos peso del byte de la izquierda son rgb */
    /* desplazamos color 8 posiciones por ello */
    /* el byte de la derecha es el carácter a escribir */
    Word ch = (Word) (c & 0x00FF) | (color << 8);
    Word *screen = (Word *)0xb8000;
    screen[(y * NUM_COLUMNS + x)] = ch;
    if (++x >= NUM_COLUMNS)
    {
      x = 0;
      y=(y+1)%NUM_ROWS;
    }
  }
}

/* para imprimir un carácter de un color */
/* funcionalidad de scroll incluida */
void printc_color_scroll(char c, char color)
{
  __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */

  if (y == NUM_ROWS)
  {
    Word *screen = (Word *)0xb8000;
    for (int i = 0; i < NUM_ROWS - 1; i++)
      for (int j = 0; j < NUM_COLUMNS; j++)
        screen[(i * NUM_COLUMNS + j)] = screen[((i + 1) * NUM_COLUMNS + j)];
    for (int j = 0; j < NUM_COLUMNS; j++)
      screen[((NUM_ROWS - 1) * NUM_COLUMNS + j)] = 0;
    --y;
  }
  if (c=='\n')
  {
    x = 0;
    ++y;
  }
  else
  {
    /* los 3 bits de menos peso del byte de la izquierda son rgb */
    /* desplazamos color 8 posiciones por ello */
    /* el byte de la derecha es el carácter a escribir */
    Word ch = (Word) (c & 0x00FF) | (color << 8);
    Word *screen = (Word *)0xb8000;
    screen[(y * NUM_COLUMNS + x)] = ch;
    if (++x >= NUM_COLUMNS)
    {
      x = 0;
      ++y;
    }
  }
}

void printc_xy(Byte mx, Byte my, char c)
{
  Byte cx, cy;
  cx=x;
  cy=y;
  x=mx;
  y=my;
  printc(c);
  x=cx;
  y=cy;
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    /* sustituido printc por print_color_scroll */
    printc_color_scroll(string[i], 0x02);
}

/* igual que printk pero se le puede pasar un char */
/* donde los 3 bits de menor peso indican el color en rgb */
void printk_color(char *string, char color)
{
  int i;
  for (i = 0; string[i]; i++)
    printc_color_scroll(string[i], color);
}
