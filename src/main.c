#include <stdio.h>

#include "lib.h"
#include "ui.h"

#define APP_ID "io.github.misterabdul.kbbi-gtk"
#define WINDOW_TITLE "KBBI Offline"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

#define LIB_KBBI_SO_PATH "./libkbbi.so"

Lib lib = NULL;

void
onDialogResponded(UI ui)
{
  UI_stop(ui);
}

void
onSearchButtonClicked(UI ui, char* query)
{
  int found = Lib_search(lib, query);
  printf("%s => %s\n", query, found ? "found" : "not found");
}

void
onListViewItemClicked(UI ui, char* word)
{
  printf("%s\n", word);
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

  UI_destroy(&ui);

  return status;
}
