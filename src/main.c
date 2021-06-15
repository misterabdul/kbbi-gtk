#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "ui.h"

#define APP_ID "io.github.misterabdul.kbbi-gtk"
#define WINDOW_TITLE "KBBI Offline"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

#define LIB_KBBI_SO_PATH "./libkbbi.so"

Lib lib = NULL;
char** resultWords = NULL;
char** resultMeans = NULL;

void
onDialogResponded(UI ui)
{
  UI_stop(ui);
}

void
onSearchButtonClicked(UI ui, char* query)
{
  UI_setWebViewContent(ui, "<div></div>");

  int found = Lib_search(lib, query);

  if (found && lib->results) {
    char** words = malloc(lib->resultSize * sizeof(char*));
    char** means = malloc(lib->resultSize * sizeof(char*));

    Results results = lib->results;
    for (int i = 0; i < lib->resultSize; i++) {
      if (!results)
        break;

      if (results->katakunci)
        words[i] = results->katakunci;

      if (results->artikata)
        means[i] = results->artikata;

      results = results->next;
    }

    UI_setListViewItems(ui, words, lib->resultSize);

    if (resultWords)
      free(resultWords);
    resultWords = words;

    if (resultMeans)
      free(resultMeans);
    resultMeans = means;

    Lib_freeResult(&lib);
  }
}

void
onListViewItemClicked(UI ui, char* word, int index)
{
  UI_setWebViewContent(ui, resultMeans[index]);
}

void
onAppRunning(UI ui)
{
  int isLoaded = Lib_load(&lib, LIB_KBBI_SO_PATH);
  if (!isLoaded)
    UI_showDialog(ui,
                  "Kesalahan",
                  "Tidak dapat memuat file \"libkbbi.so\"",
                  onDialogResponded);

  UI_onListViewItemClicked(ui, onListViewItemClicked);
  UI_onSearchButtonClicked(ui, onSearchButtonClicked);
}

int
main(int argc, char** argv)
{
  UI ui = UI_init(APP_ID, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

  int status = UI_run(ui, argc, argv, onAppRunning);

  if (lib)
    Lib_close(&lib);

  if (ui)
    UI_destroy(&ui);

  return status;
}
