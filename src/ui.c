#include <gtk/gtk.h>
#include <stdlib.h>
#include <webkitgtk-4.0/webkit2/webkit2.h>

#include "ui.h"

typedef struct _private
{
  char* appId;
  char* title;
  int initialWidth;
  int initialHeight;

  GtkApplication* app;

  GtkWidget* rootContainer;
  GtkWidget* gridContainer;
  GtkWidget* searchContainer;
  GtkWidget* scrollContainer;

  GtkWidget* searchInput;
  GtkWidget* searchButton;
  GtkWidget* listView;
  GtkWidget* webView;
} * Private;

GtkWidget*
initRootContainer(const GtkApplication* app,
                  const char* title,
                  int width,
                  int height)
{
  GtkWidget* rootContainer = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_default_size(GTK_WINDOW(rootContainer), width, height);
  gtk_window_set_title(GTK_WINDOW(rootContainer), title);
  gtk_window_set_resizable(GTK_WINDOW(rootContainer), TRUE);

  return rootContainer;
}

GtkWidget*
initGridContainer(void)
{
  GtkWidget* gridContainer = gtk_grid_new();
  gtk_grid_insert_column(GTK_GRID(gridContainer), 1);
  gtk_grid_insert_row(GTK_GRID(gridContainer), 1);

  return gridContainer;
}

GtkWidget*
initSearchContainer(void)
{
  GtkWidget* searchContainer = gtk_grid_new();
  gtk_grid_insert_column(GTK_GRID(searchContainer), 1);

  return searchContainer;
}

GtkWidget*
initScrollContainer(void)
{
  GtkWidget* scrolledWindowContainer = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_min_content_width(
    GTK_SCROLLED_WINDOW(scrolledWindowContainer), 200);
  return scrolledWindowContainer;
}

GtkWidget*
initSearchInput(void)
{
  GtkWidget* searchInput = gtk_search_entry_new();
  gtk_widget_set_halign(searchInput, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand(searchInput, TRUE);

  return searchInput;
}

GtkWidget*
initSearchButton(void)
{
  GtkWidget* searchButton = gtk_button_new();
  gtk_button_set_label(GTK_BUTTON(searchButton), "Cari");

  return searchButton;
}

GtkWidget*
initListView(void)
{
  enum ListViewColumns
  {
    LV_WordColumn,
    LV_ColumnCount
  };

  GtkTreeStore* store = gtk_tree_store_new(LV_ColumnCount, G_TYPE_STRING);

  GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes(
    "Words", renderer, "text", LV_WordColumn, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);
  gtk_tree_view_set_enable_search(GTK_TREE_VIEW(tree), FALSE);
  gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(tree), FALSE);
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(tree), FALSE);
  g_object_unref(G_OBJECT(store));

  GtkTreeSelection* selection =
    gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(selection),
                              GTK_SELECTION_SINGLE);

  return tree;
}

GtkWidget*
initWebView()
{
  GtkWidget* webView = webkit_web_view_new();
  gtk_widget_set_size_request(webView, 300, 300);
  gtk_widget_set_valign(webView, GTK_ALIGN_FILL);
  gtk_widget_set_vexpand(webView, TRUE);
  webkit_web_view_load_html(WEBKIT_WEB_VIEW(webView), "<div></div>", NULL);

  return webView;
}

void (*appOnRunningHandler)(UI);

void
onAppActivated(GtkApplication* app, gpointer userData)
{
  UI uiInstance = (UI)userData;
  Private uiPrivateInstance = (Private)uiInstance->private;

  uiPrivateInstance->rootContainer =
    initRootContainer(app,
                      uiPrivateInstance->title,
                      uiPrivateInstance->initialWidth,
                      uiPrivateInstance->initialHeight);

  uiPrivateInstance->gridContainer = initGridContainer();
  uiPrivateInstance->searchContainer = initSearchContainer();
  uiPrivateInstance->scrollContainer = initScrollContainer();

  uiPrivateInstance->searchInput = initSearchInput();
  uiPrivateInstance->searchButton = initSearchButton();
  uiPrivateInstance->listView = initListView();
  uiPrivateInstance->webView = initWebView();

  gtk_container_add(GTK_CONTAINER(uiPrivateInstance->scrollContainer),
                    uiPrivateInstance->listView);

  gtk_grid_attach(GTK_GRID(uiPrivateInstance->searchContainer),
                  uiPrivateInstance->searchInput,
                  0,
                  0,
                  1,
                  1);
  gtk_grid_attach(GTK_GRID(uiPrivateInstance->searchContainer),
                  uiPrivateInstance->searchButton,
                  1,
                  0,
                  1,
                  1);

  gtk_grid_attach(GTK_GRID(uiPrivateInstance->gridContainer),
                  uiPrivateInstance->searchContainer,
                  0,
                  0,
                  2,
                  1);
  gtk_grid_attach(GTK_GRID(uiPrivateInstance->gridContainer),
                  uiPrivateInstance->scrollContainer,
                  0,
                  1,
                  1,
                  1);
  gtk_grid_attach(GTK_GRID(uiPrivateInstance->gridContainer),
                  uiPrivateInstance->webView,
                  1,
                  1,
                  1,
                  1);

  gtk_container_add(GTK_CONTAINER(uiPrivateInstance->rootContainer),
                    uiPrivateInstance->gridContainer);

  gtk_widget_show_all(uiPrivateInstance->rootContainer);

  if (appOnRunningHandler)
    appOnRunningHandler(uiInstance);
}

UI
UI_init(const char* appId, const char* title, const int width, const int height)
{
  Private uiPrivateInstance = malloc(sizeof(struct _private));
  uiPrivateInstance->appId = appId;
  uiPrivateInstance->title = title;
  uiPrivateInstance->initialWidth = width;
  uiPrivateInstance->initialHeight = height;
  uiPrivateInstance->rootContainer = NULL;
  uiPrivateInstance->gridContainer = NULL;
  uiPrivateInstance->searchContainer = NULL;
  uiPrivateInstance->scrollContainer = NULL;

  uiPrivateInstance->searchInput = NULL;
  uiPrivateInstance->searchButton = NULL;
  uiPrivateInstance->listView = NULL;
  uiPrivateInstance->webView = NULL;

  uiPrivateInstance->app = gtk_application_new(appId, G_APPLICATION_FLAGS_NONE);

  UI uiInstance = malloc(sizeof(struct _ui));
  uiInstance->private = uiPrivateInstance;

  return uiInstance;
}

void
UI_stop(UI uiInstance)
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->app)
      g_application_quit(G_APPLICATION(uiPrivateInstance->app));
  }
}

int
UI_run(UI uiInstance, int argc, char** argv, const void (*callback)(UI))
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->app) {
      appOnRunningHandler = callback;

      g_signal_connect(uiPrivateInstance->app,
                       "activate",
                       G_CALLBACK(onAppActivated),
                       uiInstance);

      return g_application_run(
        G_APPLICATION(uiPrivateInstance->app), argc, argv);
    }
  }

  return -1;
}

void
UI_destroy(UI* uiInstance)
{
  if (*uiInstance) {
    Private uiPrivateInstance = (Private)((*uiInstance)->private);

    if (uiPrivateInstance && uiPrivateInstance->app) {
      GtkApplication* app = uiPrivateInstance->app;
      g_object_unref(app);
      free(uiPrivateInstance);
    }

    free(*uiInstance);
    *uiInstance = NULL;
  }
}

void (*dialogCallback)(UI) = NULL;

void
dialogOnResponse(GtkDialog* dialog, int responseId, gpointer userData)
{
  UI uiInstance = (UI)userData;

  if (dialogCallback)
    dialogCallback(uiInstance);

  gtk_widget_destroy(dialog);
}

void
UI_showDialog(const UI uiInstance,
              const char* title,
              const char* message,
              const void (*responseHandler)(UI))
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->rootContainer) {
      dialogCallback = responseHandler;

      GtkWidget* dialog =
        gtk_dialog_new_with_buttons(title,
                                    uiPrivateInstance->rootContainer,
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    "OK",
                                    GTK_RESPONSE_NONE,
                                    NULL);
      GtkWidget* contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
      GtkWidget* label = gtk_label_new(message);

      g_signal_connect(
        dialog, "response", G_CALLBACK(dialogOnResponse), uiInstance);

      gtk_container_add(GTK_CONTAINER(contentArea), label);
      gtk_widget_show_all(dialog);
    }
  }
}

void (*searchButtonCallback)(UI, char*) = NULL;

void
searchButtonOnClicked(GtkButton* button, gpointer userData)
{
  UI uiInstance = (UI)userData;
  Private uiPrivateInstance = (Private)uiInstance->private;

  char* searchQuery =
    gtk_entry_get_text(GTK_ENTRY(uiPrivateInstance->searchInput));

  if (searchButtonCallback)
    searchButtonCallback(uiInstance, searchQuery);
}

void
UI_onSearchButtonClicked(const UI uiInstance, const void (*handler)(UI, char*))
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->searchButton) {
      searchButtonCallback = handler;

      g_signal_connect(uiPrivateInstance->searchButton,
                       "clicked",
                       G_CALLBACK(searchButtonOnClicked),
                       uiInstance);
    }
  }
}

void (*treeViewCallback)(UI, char*, int) = NULL;
gulong treeViewOnSelectHandlerId = 0;

void
treeSelectionOnChanged(GtkTreeSelection* selection, gpointer userData)
{
  UI uiInstance = (UI)userData;
  Private uiPrivateInstance = (Private)uiInstance->private;

  GtkTreeModel* model =
    gtk_tree_view_get_model(GTK_TREE_VIEW(uiPrivateInstance->listView));
  GtkTreeIter iter;
  char* word = NULL;
  int index = -1;

  if (model) {
    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_tree_model_get(model, &iter, 0, &word, -1);
    GtkTreePath* path = gtk_tree_model_get_path(model, &iter);
    int* indices = gtk_tree_path_get_indices(path);
    index = indices[0];
    gtk_tree_path_free(path);
  }

  if (treeViewCallback)
    treeViewCallback(uiInstance, word, index);
}

void
UI_setListViewItems(const UI uiInstance,
                    const char* items[],
                    const int itemSize)
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance) {
      enum ListViewColumns
      {
        LV_WordColumn,
        LV_ColumnCount
      };

      GtkTreeStore* store = gtk_tree_store_new(LV_ColumnCount, G_TYPE_STRING);
      GtkTreeIter iter;
      for (int i = 0; i < itemSize; i++) {
        gtk_tree_store_append(GTK_TREE_STORE(store), &iter, NULL);
        gtk_tree_store_set(
          GTK_TREE_STORE(store), &iter, LV_WordColumn, items[i], -1);
      }

      GtkTreeSelection* selection =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(uiPrivateInstance->listView));
      if (treeViewOnSelectHandlerId)
        g_signal_handler_disconnect(selection, treeViewOnSelectHandlerId);

      gtk_tree_view_set_model(GTK_TREE_VIEW(uiPrivateInstance->listView),
                              GTK_TREE_MODEL(store));

      if (treeViewCallback)
        treeViewOnSelectHandlerId = g_signal_connect(
          selection, "changed", treeSelectionOnChanged, uiInstance);

      g_object_unref(G_OBJECT(store));
    }
  }
}

void
UI_onListViewItemClicked(const UI uiInstance,
                         const void (*handler)(UI, char*, int))
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->listView) {
      treeViewCallback = handler;
      GtkTreeSelection* selection =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(uiPrivateInstance->listView));

      if (treeViewCallback) {
        treeViewOnSelectHandlerId = g_signal_connect(
          selection, "changed", G_CALLBACK(treeSelectionOnChanged), uiInstance);
      } else {
        if (treeViewOnSelectHandlerId)
          g_signal_handler_disconnect(selection, treeViewOnSelectHandlerId);
      }
    }
  }
}

void
UI_setWebViewContent(const UI uiInstance, const char* html)
{
  if (uiInstance) {
    Private uiPrivateInstance = (Private)uiInstance->private;

    if (uiPrivateInstance && uiPrivateInstance->webView) {
      webkit_web_view_load_html(
        WEBKIT_WEB_VIEW(uiPrivateInstance->webView), html, NULL);
    }
  }
}
