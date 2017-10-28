#ifndef __MEMTRACE
#define __MEMTRACE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memlog.h>
#include <memlist.h>
#include "callinfo.h"


static void *(*mallocp)(size_t size) = NULL;
/*
 * {
n_malloc += 1000;
return NULL;
}*/
static void (*freep)(void *ptr) = NULL;
static void *(*callocp)(size_t nmemb, size_t size);
static void *(*reallocp)(void *ptr, size_t size);

 

#endif

