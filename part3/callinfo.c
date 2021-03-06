//------------------------------------------------------------------------------
// Linklab: part3/callinfo.c
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

#include <stdio.h>
#include "callinfo.h"

int get_callinfo(char *fname, size_t fnlen, unsigned long long *ofs)
{
  char func_name[fnlen];
  unw_cursor_t cur;
  unw_context_t uc;
  unw_word_t offp;

 unw_getcontext(&uc);
 unw_init_local(&cur, &uc);
 // goto alloc
 if (unw_step(&cur) < 0)
   return -1;
 // goto alloc in memlist.c
 if (unw_step(&cur)<0)
   return -1;
//   fprintf(stderr, "interesting1\n");
 if (unw_step(&cur)<0)
   return -1;
 if ( unw_step(&cur)<0)
   return -1;

  unw_get_proc_name(&cur, func_name, fnlen, &offp);
  
 char key[] = "__libc_start_main";
 int i = 0;
 while(i< fnlen){
   if(func_name[i] != key[i])
     break;
   i++;
 }
 if(i == fnlen-1)
  {
  *fname++ = 'm';
  *fname++ = 'a';
  *fname++ = 'i';
  *fname = 'n';
  }
  else
  {
    i = 0;
    while(func_name != '\0' && i < fnlen)
    {
      *fname++ = *(func_name+i);
      i++;
    }
  }
  *ofs = (unsigned long long )offp;
  return 1;
}

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
