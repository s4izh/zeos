/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

struct list_head free_queue;
struct list_head ready_queue;
extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t)
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t)
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t)
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos];

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{

}

void init_task1(void)
{
}

void init_free_queue()
{
  INIT_LIST_HEAD(&free_queue);
  int i;
  for (i = 0; i < NR_TASKS; ++i)
  {
    struct task_struct new;
    list_add(&(new.anchor), &free_queue);
  }
}

void init_sched()
{
  init_free_queue();
  INIT_LIST_HEAD(&ready_queue);
}

struct task_struct* current()
{
  int ret_value;

  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void inner_task_switch(union task_union *new)
{
  page_table_entry *new_DIR = get_DIR(&new->task);

  tss.esp0 = (int)&(new->stack[KERNEL_STACK_SIZE]);
  writeMSR(0x175, 0, (unsigned long)&(new->stack[KERNEL_STACK_SIZE]));

  set_cr3(&page_table_entry);

}

