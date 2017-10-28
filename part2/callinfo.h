//------------------------------------------------------------------------------
// Linklab: part1/callinfo.h
//
// Change Log
// 2016/09/01 Bernhard Egger  created
// 2016/09/23 Bernhard Egger  adapted for fall semester
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

#ifndef __CALLINFO_H__
#define __CALLINFO_H__

#include <stddef.h> // size_t definition

//
// return the PC of the callsite to the dynamic memory management function
//
//   fname      pointer to character array to hold function name
//   fnlen      length of character array
//   ofs        pointer to offset to hold PC offset into function
//
// returns
//    0         on success
//   <0         on error
//
int get_callinfo(char *fname, size_t fnlen, unsigned long long *ofs);

#endif

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
