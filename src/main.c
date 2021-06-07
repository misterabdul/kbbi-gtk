#include <gtk/gtk.h>

#include "ui.h"

#define APP_ID "io.github.misterabdul.kbbi-gtk"
#define WINDOW_TITLE "KBBI Offline"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

static void
on_activate(GApplication* app, gpointer user_data)
{
  UI_Manipulable manipulableWidgets =
    UI_initWindow(app, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
}

int
main(int argc, char** argv)
{
  int status = 0;
  GtkApplication* app = gtk_application_new(APP_ID, G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}