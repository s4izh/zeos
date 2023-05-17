/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <p_stats.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

void * get_ebp();

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF; 
  if (permissions!=ESCRIPTURA) return -EACCES; 
  return 0;
}

void user_to_system(void)
{
  update_stats(&(current()->p_stats.user_ticks), &(current()->p_stats.elapsed_total_ticks));
}

void system_to_user(void)
{
  update_stats(&(current()->p_stats.system_ticks), &(current()->p_stats.elapsed_total_ticks));
}

int sys_ni_syscall()
{
	return -ENOSYS; 
}

int sys_getpid()
{
	return current()->PID;
}

int global_PID=1000;

int ret_from_fork()
{
  return 0;
}

int sys_fork(void)
{
  struct list_head *lhcurrent = NULL;
  union task_union *uchild;
  
  /* Any free task_struct? */
  if (list_empty(&freequeue)) return -ENOMEM;

  lhcurrent=list_first(&freequeue);
  
  list_del(lhcurrent);
  
  uchild=(union task_union*)list_head_to_task_struct(lhcurrent);
  
  /* Copy the parent's task struct to child's */
  copy_data(current(), uchild, sizeof(union task_union));
  
  /* new pages dir */
  allocate_DIR((struct task_struct*)uchild);
  
  /* Allocate pages for DATA+STACK */
  int new_ph_pag, pag, i;
  page_table_entry *process_PT = get_PT(&uchild->task);
  for (pag=0; pag<NUM_PAG_DATA; pag++)
  {
    new_ph_pag=alloc_frame();
    if (new_ph_pag!=-1) /* One page allocated */
    {
      set_ss_pag(process_PT, PAG_LOG_INIT_DATA+pag, new_ph_pag);
    }
    else /* No more free pages left. Deallocate everything */
    {
      /* Deallocate allocated pages. Up to pag. */
      for (i=0; i<pag; i++)
      {
        free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
        del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
      }
      /* Deallocate task_struct */
      list_add_tail(lhcurrent, &freequeue);
      
      /* Return error */
      return -EAGAIN; 
    }
  }

  /* Copy parent's SYSTEM and CODE to child. */
  page_table_entry *parent_PT = get_PT(current());
  for (pag=0; pag<NUM_PAG_KERNEL; pag++)
  {
    set_ss_pag(process_PT, pag, get_frame(parent_PT, pag));
  }
  for (pag=0; pag<NUM_PAG_CODE; pag++)
  {
    set_ss_pag(process_PT, PAG_LOG_INIT_CODE+pag, get_frame(parent_PT, PAG_LOG_INIT_CODE+pag));
  }

  /* mapeamos al hijo todas las páginas shared */
  /* for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag<TOTAL_PAGES; pag++) */
  /* { */
  /*   int frame = get_frame(parent_PT, pag); */
  /*   int id = -1; */
  /*   for (int i = 0; i < SHARED_PAGES && id == -1; i++) */ 
  /*   { */
  /*     if (shared_pages[i].frame == frame) */
  /*       id = i; */
  /*   } */
  /*   if (id != -1) { */
  /*     set_ss_pag(process_PT, pag, frame); */
  /*     shared_pages[id].references++; */
  /*   } */
  /* } */

  /* int frames[NUM_PAG_DATA]; */
  /* int t = 0; */

  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag<NUM_PAG_KERNEL+NUM_PAG_CODE+(2*NUM_PAG_DATA); pag++)
  {
    int frame = get_frame(parent_PT, pag);
    int found = 0;
    for (int i = 0; i < SHARED_PAGES && !found; i++) 
    {
      if (shared_pages[i].frame == frame) {
        frames[t] = frame;
        found = 1;
      }
    }
    if (!found)
      frames[t] = -1;
    t++;
  }

  for (int i=0; i < NUM_PAG_DATA; ++i)
    free_frame()

  /* Copy parent's DATA to child. We will use TOTAL_PAGES-1 as a temp logical page to map to */
  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE; pag<NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag++)
  {
    set_ss_pag(parent_PT, pag+NUM_PAG_DATA, get_frame(process_PT, pag));
    copy_data((void*)(pag<<12), (void*)((pag+NUM_PAG_DATA)<<12), PAGE_SIZE);
    del_ss_pag(parent_PT, pag+NUM_PAG_DATA);
  }


  /* for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag<TOTAL_PAGES; pag++) */
  /* { */
  /*   int frame = get_frame(parent_PT, pag); */
  /*   int id = -1; */
  /*   for (int i = 0; i < SHARED_PAGES && id == -1; i++) */ 
  /*   { */
  /*     if (shared_pages[i].frame == frame) */
  /*       id = i; */
  /*   } */
  /*   if (id != -1) { */
  /*     set_ss_pag(process_PT, pag, frame); */
  /*     shared_pages[id].references++; */
  /*   } */
  /* } */

  int k = 0;
  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag<NUM_PAG_KERNEL+NUM_PAG_CODE+(2*NUM_PAG_DATA); pag++)
  {
    if (frames[k] != -1) {
      set_ss_pag(parent_PT, pag, frames[k]);
      int found = 0;
      for (int i = 0; i < SHARED_PAGES && !found; i++) 
      {
        if (shared_pages[i].frame == frames[k]) {
          shared_pages[i].references++;
          found = 1;
        }
      }
    }
    k++;
  }

  /* remapeamos las 20 primeras páginas del padre iterando sobre */
  /* las 20 primeras páginas del hijo */
  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+(2*NUM_PAG_DATA); pag<TOTAL_PAGES; pag++)
  {
    int frame = get_frame(process_PT, pag);
    int id = -1;
    for (int i = 0; i < SHARED_PAGES && id == -1; i++) 
    {
      if (shared_pages[i].frame == frame)
        id = i;
    }
    if (id != -1) {
      set_ss_pag(parent_PT, pag, frame);
    }
  }

  /* pag = NUM_PAG_KERNEL+NUM_PAG_CODE; */
  /* unsigned pag_to_copy = pag; */

  /* while (pag_to_copy<NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA) */
  /* { */
  /*   if (is_addr_free(parent_PT, (void*)(pag+NUM_PAG_DATA<<12))) { */
  /*     set_ss_pag(parent_PT, pag+NUM_PAG_DATA, get_frame(process_PT, pag_to_copy)); */
  /*     copy_data((void*)(pag_to_copy<<12), (void*)((pag+NUM_PAG_DATA)<<12), PAGE_SIZE); */
  /*     del_ss_pag(parent_PT, pag+NUM_PAG_DATA); */
  /*     ++pag_to_copy; */
  /*   } */
  /*   ++pag; */
  /* } */


  /* Deny access to the child's memory space */
  set_cr3(get_DIR(current()));

  uchild->task.PID=++global_PID;
  uchild->task.state=ST_READY;

  int register_ebp;		/* frame pointer */
  /* Map Parent's ebp to child's stack */
  register_ebp = (int) get_ebp();
  register_ebp=(register_ebp - (int)current()) + (int)(uchild);

  uchild->task.register_esp=register_ebp + sizeof(DWord);

  DWord temp_ebp=*(DWord*)register_ebp;
  /* Prepare child stack for context switch */
  uchild->task.register_esp-=sizeof(DWord);
  *(DWord*)(uchild->task.register_esp)=(DWord)&ret_from_fork;
  uchild->task.register_esp-=sizeof(DWord);
  *(DWord*)(uchild->task.register_esp)=temp_ebp;

  /* Set stats to 0 */
  init_stats(&(uchild->task.p_stats));

  /* Queue child process into readyqueue */
  uchild->task.state=ST_READY;
  list_add_tail(&(uchild->task.list), &readyqueue);
  
  return uchild->task.PID;
}

#define TAM_BUFFER 512

int sys_write(int fd, char *buffer, int nbytes) {
char localbuffer [TAM_BUFFER];
int bytes_left;
int ret;

	if ((ret = check_fd(fd, ESCRIPTURA)))
		return ret;
	if (nbytes < 0)
		return -EINVAL;
	if (!access_ok(VERIFY_READ, buffer, nbytes))
		return -EFAULT;
	
	bytes_left = nbytes;
	while (bytes_left > TAM_BUFFER) {
		copy_from_user(buffer, localbuffer, TAM_BUFFER);
		ret = sys_write_console(localbuffer, TAM_BUFFER);
		bytes_left-=ret;
		buffer+=ret;
	}
	if (bytes_left > 0) {
		copy_from_user(buffer, localbuffer,bytes_left);
		ret = sys_write_console(localbuffer, bytes_left);
		bytes_left-=ret;
	}
	return (nbytes-bytes_left);
}


extern int zeos_ticks;

int sys_gettime()
{
  return zeos_ticks;
}

void sys_exit()
{  
  int i;

  page_table_entry *process_PT = get_PT(current());

  // Deallocate all the propietary physical pages
  for (i=0; i<NUM_PAG_DATA; i++)
  {
    free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
    del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
  }

  unsigned pag;

  for (pag=NUM_PAG_KERNEL+NUM_PAG_CODE+NUM_PAG_DATA; pag<TOTAL_PAGES; pag++)
  {
    int frame = get_frame(process_PT, pag);
    int id = -1;
    for (int i = 0; i < SHARED_PAGES && id == -1; i++) 
    {
      if (shared_pages[i].frame == frame)
        id = i;
    }
    if (id != -1) {
      --shared_pages[id].references;

      if (shared_pages[id].marked_to_delete && shared_pages[id].references == 0)
      {
        memset((void*)(pag<<12), 0, PAGE_SIZE);
        shared_pages[id].marked_to_delete = 0;
      }
      del_ss_pag(process_PT, pag);
    }
  }
  
  /* Free task_struct */
  list_add_tail(&(current()->list), &freequeue);
  
  current()->PID=-1;
  
  /* Restarts execution of the next process */
  sched_next_rr();
}

/* System call to force a task switch */
int sys_yield()
{
  force_task_switch();
  return 0;
}

extern int remaining_quantum;

int sys_get_stats(int pid, struct stats *st)
{
  int i;
  
  if (!access_ok(VERIFY_WRITE, st, sizeof(struct stats))) return -EFAULT; 
  
  if (pid<0) return -EINVAL;
  for (i=0; i<NR_TASKS; i++)
  {
    if (task[i].task.PID==pid)
    {
      task[i].task.p_stats.remaining_ticks=remaining_quantum;
      copy_to_user(&(task[i].task.p_stats), st, sizeof(struct stats));
      return 0;
    }
  }
  return -ESRCH; /*ESRCH */
}

int sys_read(char* c, int size)
{
  if (!access_ok(VERIFY_WRITE, c, sizeof(char*))) return -EFAULT;
  if (size < 0) return -EINVAL;
  return read_buff(c, size);
}

int sys_gotoxy(int x, int y)
{
  return gotoxy(x, y);
}

int sys_set_color(int fg, int bg)
{
  return set_color(fg, bg);
}

void* sys_shmat(int id, void *addr)
{
  if (id < 0 || id > 9) 
    return -EINVAL;
  if ((unsigned long)addr % PAGE_SIZE != 0)
    return -EINVAL;

  page_table_entry *PT = get_PT(current());

  if (addr == NULL || !is_addr_free(PT, addr)) {
    addr = (void*)get_free_addr(PT);
    if (addr == NULL) 
      return -ENOMEM;
  }

  ++shared_pages[id].references;

  set_ss_pag(PT, (unsigned long)addr / PAGE_SIZE, shared_pages[id].frame); 
  return (void*)addr;
}

int sys_shmdt(void *addr)
{
  if ((unsigned long)addr % PAGE_SIZE != 0)
    return -EINVAL;

  if (addr == NULL) 
    return -EINVAL;

  unsigned page = (unsigned long)addr >> 12;

  if (page < NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA || page >= TOTAL_PAGES)
    return -EFAULT;

  page_table_entry *current_PT = get_PT(current());
  if (is_addr_free(current_PT, addr))
    return -EFAULT; // la página ya esta libre

  int id = -1;
  int frame = get_frame(current_PT, page);

  for (int i = 0; i < 10 && id == -1; ++i)
  {
    if (shared_pages[i].frame == frame)
      id = i;
  }

  if (id == -1)
    return -EINVAL;

  --shared_pages[id].references;

  if (shared_pages[id].marked_to_delete && shared_pages[id].references == 0)
  {
    memset((void*)addr, 0, PAGE_SIZE);
    shared_pages[id].marked_to_delete = 0;
  }

  del_ss_pag(current_PT, page);

  set_cr3(get_DIR(current()));

  return 0;
}

// marks the shared page id to be cleaned (all 0)
// after last process unmaps the page
int sys_shmrm(int id)
{
  if (id < 0 || id > 9) 
    return -EINVAL;

  shared_pages[id].marked_to_delete = 1;
  return 0;
}
