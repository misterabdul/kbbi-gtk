#ifndef _UI_H
#define _UI_H

#include <gtk/gtk.h>
#include <webkitgtk-4.0/webkit2/webkit2.h>

typedef struct _ui_manipulable
{
  GtkWidget* searchInput;
  GtkWidget* listView;
  GtkWidget* webView;
} * UI_Manipulable;

UI_Manipulable
UI_initWindow(const GApplication* app,
              const char* title,
              const int width,
              const int height);

#endif
