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
    printc(string[i]);
}

#define CIRCULAR_BUFF_SIZE 64

char circular_buff[CIRCULAR_BUFF_SIZE];
int buff_full = 0;
int head, tail = 0;

void write_buff(char c) {
  if (!buff_full) {
    circular_buff[head] = c;
    head = (head + 1) % CIRCULAR_BUFF_SIZE;
    if (head == tail) buff_full = 1;
  }
}

int read_buff(char* c, int size) {
  // empty buffer
  if ((head == tail) && (buff_full != 1)){
    return -1;
  } 

  int c_read;
  int possible_reads = head - tail;
  if (possible_reads < 0) possible_reads = CIRCULAR_BUFF_SIZE + possible_reads;

  int todo;
  if (size >= possible_reads) todo = possible_reads;
  else todo = size;

  for (c_read = 0; c_read < todo; c_read++)
  {
    *c = circular_buff[tail];
    tail = (tail + 1) % CIRCULAR_BUFF_SIZE;
    c++; 
  }
  buff_full = 0;
  return c_read;
}
