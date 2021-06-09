#ifndef _UI_H
#define _UI_H

typedef struct _ui
{
  void* private;
} * UI;

UI
UI_init(const char* appId,
        const char* title,
        const int width,
        const int height);

int
UI_run(UI ui, int argc, char** argv, const void (*onAppRunning)(UI));

void
UI_destroy(UI* ui);

void
UI_showDialog(const UI ui, const char* title, const char* message);

void
UI_onSearchButtonClicked(const UI ui, const void (*handler)(char*));

void
UI_onListViewItemClicked(const UI ui, const void (*handler)(char*));

#endif
