/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

#define BUFF_SIZE 4096

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  if (list_empty(&free_queue))
    return -ENOMEM;

  struct list_head *free_queue_head = list_first(&free_queue);
  list_del(free_queue_head);

  struct task_struct *child_task_s = list_head_to_task_struct(free_queue_head);
  union task_union *child_task_u = (union task_union *)child_task_s;

  struct task_struct *parent_task_s = current();
  union task_union *parent_task_u = (union task_union *)parent_task_s;

  copy_data(parent_task_u, child_task_u, sizeof(union task_union));

  allocate_DIR(child_task_s);

  page_table_entry *child_PT = get_PT(child_task_s);

  int page = 0;
  int new_frame;

  for (page = 0; page < NUM_PAG_DATA; ++page)
  {
    new_frame = alloc_frame();
    if (new_frame != -1)
    {
      set_ss_pag(child_PT, PAG_LOG_INIT_DATA + page, new_frame);
    }
    else // dealocatar todo
    {
      // hacemos el bucle hacia atrás
      for (; page >= 0; --page)
      {
        free_frame(get_frame(child_PT, PAG_LOG_INIT_DATA + page));
        del_ss_pag(child_PT, PAG_LOG_INIT_DATA + page);
      }
      list_add(free_queue_head, &free_queue);
      return -EAGAIN;
    }
  }

  // creates the child process

  return PID;
}

void sys_exit()
{
}

int sys_write(int fd, char *buffer, int size)
{
    int check = check_fd(fd, ESCRIPTURA);
    if (check != 0)
        return check;
    if (buffer == NULL)
        return -EFAULT;
    if (!(access_ok(VERIFY_WRITE, buffer, size)))
        return -EFAULT;
    if (size <= 0)
        return -EINVAL; // parámetros inválido

    char buff[BUFF_SIZE]; // 4096
    int bytesleft = size;
    int byteswritten = 0;

    while (bytesleft > BUFF_SIZE) {
        copy_from_user(buffer, buff, BUFF_SIZE);
        byteswritten += sys_write_console(buff, BUFF_SIZE);
        bytesleft -= BUFF_SIZE;
    }
    if (bytesleft) {
        copy_from_user(buffer, buff, bytesleft);
        byteswritten += sys_write_console(buff, bytesleft);
    }
    return byteswritten;
}

extern int zeos_ticks;

int sys_gettime() 
{
  return zeos_ticks;
}
