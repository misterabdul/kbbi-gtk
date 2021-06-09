#include <stdio.h>

#include "ui.h"

#define APP_ID "io.github.misterabdul.kbbi-gtk"
#define WINDOW_TITLE "KBBI Offline"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

void
onSearchButtonClicked(char* query)
{
  printf("%s\n", query);
}

void
onListViewItemClicked(char* word)
{
  printf("%s\n", word);
}

void
onAppRunning(UI ui)
{
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
