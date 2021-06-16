#ifndef _LIB_H
#define _LIB_H

/**
 * The results struct.
 */
typedef struct _results
{
  char* katakunci;
  char* artikata;
  struct _results* next;
} * Results;

/**
 * The lib struct.
 */
typedef struct _lib
{
  Results results;
  int resultSize;
  void* private;
} * Lib;

/**
 * Load the lib from given path.
 *
 * @param[out]  lib   The lib instance.
 * @param[in]   path  The lib's path.
 * @return            Result of the load lib process.
 */
int
Lib_load(Lib* lib, const char* path);

/**
 * Close the lib.
 *
 * @param[out]  lib The lib instance.
 */
void
Lib_close(Lib* lib);

/**
 * Free the result instance.
 *
 * @param[out]  lib The lib instance.
 */
void
Lib_freeResult(Lib* lib);

/**
 * Do search on lib with given query.
 *
 * @param[in] lib   The lib instance.
 * @param[in] query The search query.
 * @return          Status of the search (found or not found).
 */
int
Lib_search(Lib lib, const char* query);

/**
 * Count all words inside the lib.
 *
 * @param[in] lib The lib instance.
 * @return        Total number of words inside the lib.
 */
int
Lib_count(Lib lib);

/**
 * Get the current lib version.
 *
 * @param[in] lib The lib instance.
 * @return        The current lib version string.
 */
char*
Lib_version(Lib lib);

#endif
