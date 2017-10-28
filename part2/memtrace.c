//------------------------------------------------------------------------------
// Linklab: part1/memtrace.c
//
// Change Log
// 2016/09/01 Bernhard Egger  created
// 2016/09/23 Bernhard Egger  adapted for 2016 fall semester
// 2017/10/01 Bernhard Egger  adapted for 2017 fall semester
//
// License
// Copyright (c) 2016-2017, Computer Systems and Platforms Laboratory
// All rights reserved.
//
// Redistribution and use in source and binary forms,  with or without modifi-
// cation, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF MERCHANTABILITY  AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER  OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT,  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSE-
// QUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT
// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// memtrace
//
// trace calls to the dynamic memory manager
//
#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memlog.h>
#include <memlist.h>
#include "callinfo.h"
#include "memtrace.h"

//
// function pointers to stdlib's memory management functions
//
//
// statistics & other global variables
//
//static unsigned long n_malloc  = 0;
//static unsigned long n_calloc  = 0;
//static unsigned long n_realloc = 0;
static unsigned long n_allocb  = 0;
static unsigned long n_alloc   = 0;
static unsigned long n_freeb   = 0;
static unsigned long n_free    = 0;
static item *list = NULL;



//
// init - this function is called once when the shared library is loaded
//
__attribute__((constructor))
void init(void)
{
  char *error;

  LOG_START();

  // initialize a new static itemlist to keep track of all memory (de-)allocations
  // (not needed for part 1)
  list = new_list();
  freep = dlsym(RTLD_NEXT, "free");
  if((error = dlerror())!= NULL)
  {
    fprintf(stderr, "dlsym error: %s \n", error);
    exit(1);
  }

  mallocp = dlsym(RTLD_NEXT, "malloc");
  if((error = dlerror())!= NULL)
  {
    fprintf(stderr, "dlsym error: %s \n", error);
    exit(1);
  }
  callocp = dlsym(RTLD_NEXT, "calloc");
 if((error = dlerror())!= NULL)
  {
    fprintf(stderr, "dlsym error: %s \n", error);
    exit(1);
  }
  reallocp = dlsym(RTLD_NEXT, "realloc");
  if((error = dlerror())!= NULL)
  {
    fprintf(stderr, "dlsym error: %s \n", error);
    exit(1);
  }
  callocp = dlsym(RTLD_NEXT, "calloc");
  if((error = dlerror())!= NULL)
  {
    fprintf(stderr, "dlsym error: %s \n", error);
    exit(1);
  }
}

//
// realloc
//
void *realloc(void *ptr, size_t size)
{
  assert(ptr!=NULL);
  void* ret_ptr = reallocp(ptr, size);
  LOG_REALLOC(ptr, size, ret_ptr);

// add free size of previous size
  item* handler = (item*)find(list, ptr);
  n_free++;
  n_freeb += handler->size;

// update the block item with the new size
  remove_from_list(list, ptr);
  item *new_malloc_item = alloc(list, ret_ptr, size);

  n_allocb += size;
  n_alloc++;
  freep(handler);
  return ret_ptr;
}

//
// calloc
//
void *calloc(size_t nmenb, size_t s)
{
  size_t size = nmenb * s;
  void * ret_ptr = callocp(nmenb, s);
  LOG_CALLOC(nmenb, s, ret_ptr);

  item *new_malloc_item = alloc(list, ret_ptr, size);

  n_alloc++;
  n_allocb += size;
  return ret_ptr;
}

//
// free
//
void free(void *ptr)
{
  assert(ptr!=NULL);
  // first check if it's valid
  if(find(list, ptr) == NULL)
  { 
    LOG_ILL_FREE();
    return ;
  }
  //
  // then check if we got too many
  if(n_alloc <= n_free)
  {
    LOG_DOUBLE_FREE();
    n_free++;
    return ;
  }
  item* handler = (item*)find(list, ptr);
  handler->flag = 0;
  remove_from_list(list, ptr);
  n_free++;
  n_freeb += handler->size;
  freep(ptr);
  freep(handler);
  LOG_FREE(ptr);
}

//
// malloc
//
void * malloc(size_t size)
{ 
  assert(size != 0);
  void* ret_ptr = mallocp(size);
  LOG_MALLOC(size, ret_ptr);

  item *new_malloc_item = alloc(list, ret_ptr, size);

  n_alloc++;
  n_allocb += size;

  return ret_ptr;
}

void dump_nonfreed(item *list)
{
  item* temp = list;
  if (list == NULL)
  {
    fprintf(stderr, "list is empty\n");
    return ;
  }

  temp = temp->next;
  while ((temp != NULL) ) {
    
  //  if (handler->flag == 1)
       LOG_BLOCK(temp->ptr, temp->size, temp->cnt, temp->fname, temp->ofs);
    temp = temp->next;
//    fprintf(stderr, "in the dump loop, flag =%d\n", handler->flag);
  }

  return ;
}
//
// fini - this function is called once when the shared library is unloaded
//
__attribute__((destructor))
void fini(void)
{

  unsigned long alloc_total = n_allocb; 
  unsigned long allocated_avg = n_allocb / n_alloc;
  LOG_STATISTICS(alloc_total, allocated_avg, n_freeb);

// dump_list(list);

  LOG_NONFREED_START();
  dump_nonfreed(list);

  LOG_STOP();

  // free list (not needed for part 1)
  free_list(list);
}

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
