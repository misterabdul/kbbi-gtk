#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

/**
 * The Lib's private data struct.
 */
typedef struct _private
{
  void* soHandler;
  Results (*initResult)(void);
  void (*freeResult)(Results);
  int (*search)(Results*, int*, char*, int);
  int (*count)(void);
  char* (*version)(void);
} * Private;

int
Lib_load(Lib* lib, const char* path)
{
  void* soHandler = dlopen(path, RTLD_LAZY);
  if (!soHandler)
    return 0;

  /* clear any existing eror */
  dlerror();

  Results (*initResult)(void);
  void (*freeResult)(Results);
  int (*search)(Results*, int*, char*, int);
  int (*count)(void);
  char* (*version)(void);

  *(void**)(&initResult) = dlsym(soHandler, "init_result");
  *(void**)(&freeResult) = dlsym(soHandler, "free_result");
  *(void**)(&search) = dlsym(soHandler, "search");
  *(void**)(&count) = dlsym(soHandler, "count");
  *(void**)(&version) = dlsym(soHandler, "version");

  if (dlerror())
    return 0;

  Private libPrivateInstance = malloc(sizeof(struct _private));
  libPrivateInstance->soHandler = soHandler;
  libPrivateInstance->initResult = initResult;
  libPrivateInstance->freeResult = freeResult;
  libPrivateInstance->search = search;
  libPrivateInstance->count = count;
  libPrivateInstance->version = version;

  Lib libInstance = malloc(sizeof(struct _lib));
  libInstance->results = NULL;
  libInstance->resultSize = 0;
  libInstance->private = libPrivateInstance;
  *lib = libInstance;

  return 1;
}

/**
 * Custom function to free the result recursively.
 *
 * @param[in] result
 */
void
freeResult(Results result)
{
  if (result) {
    // if (result->artikata)
    //   free(result->artikata);
    // if (result->katakunci)
    //   free(result->katakunci);
    freeResult(result->next);
    free(result);
  }
}

void
Lib_freeResult(Lib* libInstance)
{
  if (*libInstance)
    freeResult((*libInstance)->results);

  (*libInstance)->results = NULL;
  (*libInstance)->resultSize = 0;
}

void
Lib_close(Lib* libInstance)
{
  if (*libInstance) {
    Private libPrivateInstance = (Private)(*libInstance)->private;

    if (libPrivateInstance && libPrivateInstance->soHandler) {
      dlclose(libPrivateInstance->soHandler);
      free(libPrivateInstance);
    }
  }

  free(*libInstance);
  *libInstance = NULL;
}

int
Lib_search(Lib libInstance, const char* query)
{
  int status = 0;

  if (libInstance && query) {
    Private libPrivateInstance = (Private)libInstance->private;

    if (libPrivateInstance && libPrivateInstance->search &&
        libPrivateInstance->initResult && libPrivateInstance->freeResult) {
      Results results = libPrivateInstance->initResult();
      int resultSize = 0;
      status =
        libPrivateInstance->search(&results, &resultSize, query, strlen(query));

      if (libInstance->results)
        libPrivateInstance->freeResult(libInstance->results);
      libInstance->results = results;
      libInstance->resultSize = resultSize;
    }
  }

  return status;
}

int
Lib_count(Lib libInstance)
{
  if (libInstance) {
    Private libPrivateInstance = (Private)libInstance->private;

    if (libPrivateInstance && libPrivateInstance->count) {
      return libPrivateInstance->count();
    }
  }

  return 0;
}

char*
Lib_version(Lib libInstance)
{
  if (libInstance) {
    Private libPrivateInstance = (Private)libInstance->private;

    if (libPrivateInstance && libPrivateInstance->version) {
      return libPrivateInstance->version();
    }
  }

  return NULL;
}
