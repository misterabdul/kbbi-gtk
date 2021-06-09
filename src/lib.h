#ifndef _LIB_H
#define _LIB_H

typedef struct _lib
{
  void* private;
} * Lib;

int
Lib_load(Lib* lib, const char* path);

void
Lib_close(Lib* lib);

#endif
