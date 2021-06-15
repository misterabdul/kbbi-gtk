#ifndef _LIB_H
#define _LIB_H

typedef struct _results
{
  char* katakunci;
  char* artikata;
  struct _results* next;
} * Results;

typedef struct _lib
{
  Results results;
  int resultSize;
  void* private;
} * Lib;

int
Lib_load(Lib* lib, const char* path);

void
Lib_close(Lib* lib);

void
Lib_freeResult(Lib* lib);

int
Lib_search(Lib lib, const char* query);

int
Lib_count(Lib lib);

char*
Lib_version(Lib lib);

#endif
