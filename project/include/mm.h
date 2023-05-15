/*
 * mm.h - Capçalera del mòdul de gestió de memòria
 */

#ifndef __MM_H__
#define __MM_H__

#include <types.h>
#include <mm_address.h>
#include <sched.h>

 
#define FREE_FRAME 0
#define USED_FRAME 1
/* Bytemap to mark the free physical pages */
extern Byte phys_mem[TOTAL_PAGES];

extern int mapped_shared_pages_count[SHARED_PAGES];
extern int marked_to_reset[SHARED_PAGES];

extern page_table_entry dir_pages[NR_TASKS][TOTAL_PAGES];

int init_frames( void );
int alloc_frame( void );
void free_frame( unsigned int frame );
void set_user_pages( struct task_struct *task );


extern Descriptor  *gdt;

extern TSS         tss; 

void init_mm();
void set_cr3(page_table_entry *dir);

void setGdt();

void setTSS();

void set_ss_pag(page_table_entry *PT, unsigned page,unsigned frame);
void del_ss_pag(page_table_entry *PT, unsigned page);
unsigned int get_frame(page_table_entry *PT, unsigned int page);

int is_addr_free(page_table_entry *PT, void* addr);
void* get_free_addr(page_table_entry *PT);

#endif  /* __MM_H__ */
