//------------------------------------------------------------------------------
// Linklab: memlist.c
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

#define _GNU_SOURCE

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "callinfo.h"
#include "memlist.h"

//
// function pointers to stdlib's malloc/free implementation
//
static void *(*callocp)(size_t nelem, size_t size) = NULL;
static void (*freep)(void *ptr) = NULL;

item *new_list(void)
{
  // since we are tracing memory (de-)allocations we cannot use
  // calloc/free directly. We need to make sure that we call the
  // implementations in stdlib in order not to have them caught 
  // by our own tracer library.
  char *error;

  dlerror();

  callocp = dlsym(RTLD_NEXT, "calloc");
  if (((error = dlerror()) != NULL) || (callocp == NULL)) {
    fprintf(stderr, "Error getting symbol 'calloc': %s\n", error);
    exit(EXIT_FAILURE);
  }

  freep = dlsym(RTLD_NEXT, "free");
  if (((error = dlerror()) != NULL) || (freep == NULL)) {
    fprintf(stderr, "Error getting symbol 'free': %s\n", error);
    exit(EXIT_FAILURE);
  }

    return (item*) callocp(1, sizeof(item));
}

void free_list(item *list)
{
  if(list == NULL)
    return ;
  item *next;

  while (list) {
    next = list->next;
    freep(list);
    list = next;
  }
}

item *alloc(item *list, void *ptr, size_t size)
{
  item *prev, *cur, *i;
  int res;

  if (list == NULL) return NULL;

  i = (item*)callocp(1, sizeof(item));
  i->ptr = ptr;
  i->size = size;
  i->cnt = 1;
  i->flag = 1;
  if (get_callinfo(&i->fname[0], sizeof(i->fname), &i->ofs) < 0) {
    strncpy(i->fname, "???", sizeof(i->fname));
    i->ofs = 0;
  }

  // check if block already exists
  prev = list; cur = list->next;
  while ((cur != NULL) && (cur->ptr < i->ptr)) {
    prev = cur; cur = cur->next;
  }

  if (cur == NULL) {
    // new block -> insert into list
    prev->next = i;
  } else {
    if (cur->ptr == i->ptr) {
      // existing block -> update size & reference counter
      cur->size = i->size;
      cur->cnt++;
      freep(i);
      i = cur;
    } else {
      // new block -> insert into list
      i->next = cur;
      prev->next = i;
    }
  }

  return i;
}

item *dealloc(item *list, void *ptr)
{
  item *prev, *cur, *i;

  if (list == NULL) return NULL;

  // find block
  prev = list; cur = list->next;
  while ((cur != NULL) && (cur->ptr != ptr)) {
    prev = cur; cur = cur->next;
  }

  // decrement reference count if found
  if (cur != NULL) cur->cnt--;
  return cur;
}

item *find(item *list, void *ptr)
{
  if (list == NULL) 
  {
    fprintf(stderr, "NULL list\n");
    return NULL;
  }

  list = list->next;
  while ((list != NULL) && (ptr != list->ptr)) {
    list = list->next;
  }

  if ((list != NULL) && (list->ptr == ptr)) return list;
  else return NULL;
}


void remove_from_list(item *list, void *ptr)
{
   item *prev, *cur, *i;

  if (list == NULL) return ;

  // find block
  prev = list; cur = list->next;
  while ((cur != NULL) && (cur->ptr != ptr)) {
    prev = cur; cur = cur->next;
  }

  // remove object if found
  if (cur != NULL) {
    if(cur->next != NULL)
      prev->next = cur->next;
    else
      prev->next = NULL;
  }
}

void dump_list(item *list)
{
  assert(list != NULL);
  item *i = list->next;

  printf("  %-16s   %-8s   %-3s   %-16s\n",
      "block", "size", "cnt", "caller");
  while (i != NULL) {
  fprintf(stderr, "flag = %d\n", i->flag);
    if(i->flag == 0)
    {
      i = i->next;
      continue;
    }
    printf("  %-16p   %-8zd   %-3d   %s:%llx\n",
        i->ptr, i->size, i->cnt, i->fname, i->ofs);
    i = i->next;
  }
}

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
