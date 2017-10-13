//------------------------------------------------------------------------------
// Linklab: memlist.h
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

#ifndef __MEMLIST_H__
#define __MEMLIST_H__

#include <stddef.h>

//
// list element holding information about an allocated memory block
//
//   ptr        pointer to block
//   size       size of block
//   cnt        allocate count
//   fname      name of function allocating block
//   ofs        offset of instruction in function
//
//   next       pointer to next item in linked list
//
typedef struct __item {
  void *ptr;
  size_t size;
  int cnt;
  char fname[32];
  unsigned long long ofs;
  struct __item *next;
} item;


//
// initialize a new list.
// The first element is a dummy element
//
item *new_list(void);

//
// free a list
//
void free_list(item *list);

//
// add information about newly allocated block to list
//
//
//   list       pointer to list
//   ptr        pointer to newly allocated block
//   size       size of newly allocated block
//
// returns
//    item*     pointer to item holding information about the block
//
// the reference count is updated automatically. If the block is re-allocated
// (i.e., there already is an item to ptr) the size of the item is adjusted.
//
item *alloc(item *list, void *ptr, size_t size);

//
// update information on freed block
//
//    list      pointer to list
//    ptr       pointer to freed block
//
// returns
//    item*     pointer to item holding information about freed block
//
// the reference count is updated automatically
//
item *dealloc(item *list, void *ptr);

//
// find information about a block in list
//
//   list       pointer to list
//   ptr        pointer of block
//
// returns
//    item*     pointer to item holding information about the block
//
item *find(item *list, void *ptr);

//
// dump (print) the list in human-readable form to stdout
//
//   list       pointer to list
//
void dump_list(item *list);

#endif

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
