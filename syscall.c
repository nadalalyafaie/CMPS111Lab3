/* 
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The 
 * Nachos copyright notice is reproduced in full below. 
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017 David C. Harrison. All rights reserved.
 */

#include <stdio.h>
#include <stdint.h>
#include <syscall-nr.h>
#include <list.h>
#include <string.h>

#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/umem.h"
#include "user/syscall.h"


static void syscall_handler(struct intr_frame *);

static void write_handler(struct intr_frame *);
static void exit_handler(struct intr_frame *);


static void create_handler(struct intr_frame *f);
static void open_handler(struct intr_frame *f);
static void read_handler(struct intr_frame *f);
static void close_handler(struct intr_frame *f);
static void exec_handler(struct intr_frame *f);
static void wait_handler(struct intr_frame *f);


void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f)
{
  int syscall;
  ASSERT( sizeof(syscall) == 4 ); // assuming x86

  // The system call number is in the 32-bit word at the caller's stack pointer.
  umem_read(f->esp, &syscall, sizeof(syscall));

  // Store the stack pointer esp, which is needed in the page fault handler.
  // Do NOT remove this line
  thread_current()->current_esp = f->esp;

  switch (syscall) {
  case SYS_HALT: 
    shutdown_power_off();
    break;

  case SYS_EXIT: 
    exit_handler(f);
    break;
      
  case SYS_WRITE: 
    write_handler(f);
    break;
    
  case SYS_CREATE:
    create_handler(f);
    break;
    
  case SYS_OPEN:
    open_handler(f);
    break;
    
  case SYS_READ:
    read_handler(f);
    break;
    
  case SYS_CLOSE:
    close_handler(f);
    break;
    
  case SYS_EXEC:
    exec_handler(f);
    break;
    
  case SYS_WAIT:
    wait_handler(f);
    break;
          
      

  default:
    printf("[ERROR] system call %d is unimplemented!\n", syscall);
    thread_exit();
    break;
  }
}

/****************** System Call Implementations ********************/

// *****************************************************************
// CMPS111 Lab 3 : Put your new system call implementatons in your 
// own source file. Define them in your header file and include 
// that .h in this .c file.
// *****************************************************************


void sys_exit(int status) 
{
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

static void exit_handler(struct intr_frame *f) 
{
  int exitcode;
  umem_read(f->esp + 4, &exitcode, sizeof(exitcode));

  sys_exit(exitcode);
}

/*
 * BUFFER+0 and BUFFER+size should be valid user adresses
 */
static uint32_t sys_write(int fd, const void *buffer, unsigned size)
{
  umem_check((const uint8_t*) buffer);
  umem_check((const uint8_t*) buffer + size - 1);

  int ret = -1;

  if (fd == 1) { // write to stdout
    putbuf(buffer, size);
    ret = size;
  }

  return (uint32_t) ret;
}

static void write_handler(struct intr_frame *f)
{
    int fd;
    const void *buffer;
    unsigned size;

    umem_read(f->esp + 4, &fd, sizeof(fd));
    umem_read(f->esp + 8, &buffer, sizeof(buffer));
    umem_read(f->esp + 12, &size, sizeof(size));

    f->eax = sys_write(fd, buffer, size);
}

//NEW FUNCTIONS---------------------- 

//The handler takes off the arguements from the stack and calls the system call
//function that will run it

/* - CREATE - */

int sys_create(char *name, size_t size) {
    //printf("create called '%s'", name);
    
    //create copy of string
    char *copyName = (char *) palloc_get_page(0);
    bool didCreate;
    
    strlcpy(copyName, name, PGSIZE);
    didCreate = filesys_create(copyName,size,false);
    palloc_free_page(copyName);
    
    return didCreate;
}

static void create_handler(struct intr_frame *f){
    char *name;
    size_t size;
    
    umem_read(f->esp + 4, &name, sizeof(name));
    umem_read(f->esp + 8, &size, sizeof(size));
    
    f->eax = sys_create(name, size);
}

/* - FOPEN - */

int sys_open(const char *restrict filename, const char *restrict mode){
    printf("open called");
}

static void open_handler(struct intr_frame *f){
    const char *restrict filename;
    const char *restrict mode;
    
    umem_read(f->esp + 4, &filename, sizeof(filename));
    umem_read(f->esp + 8, &mode, sizeof(mode));
    
    f->eax = sys_open(filename, mode);
}

/* - FREAD - */

uint32_t sys_read(int fd, void *buf, size_t count){
    printf("read called");
}

static void read_handler(struct intr_frame *f){
    int fd;
    void *buf;
    size_t count;
    
    umem_read(f->esp + 4, &fd, sizeof(fd));
    umem_read(f->esp + 8, &buf, sizeof(buf));
    umem_read(f->esp + 12, &count, sizeof(count));
    
    f->eax = sys_read(fd, buf, count);
}

/* - CLOSE - */

int sys_close(int stream){
    printf("close called");
}

static void close_handler(struct intr_frame *f){
    int stream; //Was FILE *
    
    umem_read(f->esp + 4, &stream, sizeof(stream));
    
    f->eax = sys_close(stream);
}

/* - EXEC - */

pid_t sys_exec(char *name) {
    printf("exec called");
    return 0;
}

static void exec_handler(struct intr_frame *f){
    char *name;
    
    umem_read(f->esp + 4, &name, sizeof(name));
    
    f->eax = sys_exec(name);
}

/* - WAIT - */

int sys_wait(pid_t process) {
    printf("wait called");
    return 0;
}

static void wait_handler(struct intr_frame *f){
    pid_t process;
    
    
    umem_read(f->esp + 4, &process, sizeof(process));
    
    f->eax = sys_wait(process);
}


