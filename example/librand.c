/* librand.c */

#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define YOUR_NUM 4
#define HLINE "-------------------------------------------"

int rand(void) {
  static int (*randp)(void);  // function pointer to original rand func
  int num;                    // rand() result

  char *error;
  dlerror();

  randp = dlsym(RTLD_NEXT, "rand");
  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "Dynamic Linking Error: %s\n", error);
    exit(EXIT_FAILURE);
  }

  // call the actual stdlib rand() function
  num = randp() % 6 + 1;

  printf("\n%s\n", HLINE);
  printf("| %s: %s Intercepted!            |\n", __FILE__, __func__);
  printf("| %s: Random number returns %d.     |\n", __FILE__, num);
  printf("| %s: But it will change to %d. ;)  |\n", __FILE__, YOUR_NUM);
  printf("%s\n", HLINE);

  num = YOUR_NUM;

  return num;
}

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
