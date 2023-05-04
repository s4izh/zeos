/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

/** Screen functions **/
/**********************/

Byte inb (unsigned short port);
void printc(char c);
void printc_xy(Byte x, Byte y, char c);
void printk(char *string);

void write_buff(char c);
int read_buff(char *c, int size);
int gotoxy(int _x, int _y);
int set_color(int fg, int bg);

#endif  /* __IO_H__ */
