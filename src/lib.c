#include <dlfcn.h>
#include <stdlib.h>

#include "lib.h"

int
Lib_load(Lib* lib, const char* path)
{
  void* soHandler = dlopen(path, RTLD_LAZY);
  if (!soHandler)
    return 0;

  Lib libInstance = malloc(sizeof(struct _lib));
  libInstance->private = soHandler;
  *lib = libInstance;

  return 1;
}

void
Lib_close(Lib* lib)
{
  dlclose((*lib)->private);
  free(*lib);
  *lib = NULL;
}
