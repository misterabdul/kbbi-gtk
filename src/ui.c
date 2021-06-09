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
  GtkTreeIter iter;
  for (int i = 0; i < 100; i++) {
    gtk_tree_store_append(store, &iter, NULL);
    gtk_tree_store_set(store, &iter, LV_WordColumn, "lorem", -1);
  }

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
  webkit_web_view_load_html(
    WEBKIT_WEB_VIEW(webView),
    "<div>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi "
    "malesuada at sapien ultricies tristique. Vivamus in ipsum quam. Nam "
    "facilisis quam sed nulla ullamcorper, molestie auctor mi tincidunt. Etiam "
    "euismod malesuada nibh. Aenean id semper nibh. Aliquam maximus porttitor "
    "odio condimentum malesuada. Vestibulum vel lacus at justo vehicula "
    "hendrerit. Quisque molestie quam eget ipsum sagittis cursus. Nulla eget "
    "nunc odio. Nullam vitae volutpat leo. Aliquam dignissim leo nec est "
    "varius, in pharetra nisl vulputate. Nam malesuada maximus odio, sed "
    "lobortis nisl scelerisque efficitur. Nullam ultrices eget sem eget "
    "luctus. Suspendisse id dolor arcu. Mauris ultrices orci in est viverra "
    "interdum. Etiam mollis massa ac porta venenatis."
    "Duis non felis nec tellus consectetur congue. Donec ac arcu enim. Ut "
    "mollis nunc pretium eros imperdiet, vel faucibus mauris interdum. "
    "Curabitur vehicula ullamcorper viverra. Quisque congue magna et lorem "
    "varius eleifend. Ut non turpis non elit congue porttitor vel vel ligula. "
    "Cras euismod a urna id ullamcorper. Ut eu ex dictum, tristique ligula "
    "vitae, fermentum justo. Cras mattis, leo sit amet pellentesque fringilla, "
    "purus augue vestibulum nisi, eu ultrices orci dui in mi."
    "Nulla suscipit nec urna sit amet auctor. Sed rhoncus, leo vel venenatis "
    "facilisis, ex odio molestie urna, eu mattis dui risus a massa. Nulla "
    "consequat libero neque, id aliquam erat placerat in. Duis pretium, augue "
    "vel facilisis dapibus, ligula augue accumsan nibh, vel maximus sapien "
    "nunc sed magna. Suspendisse potenti. Nullam commodo dolor eget ante "
    "pellentesque ornare. Sed elementum metus eget tellus tempor, sit amet "
    "posuere dolor tempus. Pellentesque ut consectetur ex. Mauris nibh ante, "
    "tempus non gravida vitae, fermentum ut nulla. Nulla vitae auctor mi, sit "
    "amet dictum ex. Phasellus suscipit neque id metus tempus finibus. Proin "
    "finibus metus vitae urna vehicula gravida. Etiam rutrum tincidunt "
    "interdum."
    "Donec eu elementum ipsum. Curabitur condimentum dapibus purus, vel "
    "hendrerit libero maximus at. Fusce vestibulum eleifend enim, vel "
    "tincidunt augue pharetra at. Quisque sed facilisis augue. Fusce feugiat, "
    "lorem vel tincidunt malesuada, turpis velit molestie diam, vitae pharetra "
    "erat mi sit amet leo. Maecenas id turpis eget eros elementum vestibulum. "
    "Suspendisse a leo vel tortor vulputate vulputate. Nam sem sapien, porta "
    "sed aliquet nec, convallis ut urna. Pellentesque habitant morbi tristique "
    "senectus et netus et malesuada fames ac turpis egestas."
    "In eu pretium augue, vel tempor turpis. Donec ac tristique purus, sit "
    "amet dapibus nulla. Vivamus ex ligula, bibendum a rutrum vitae, efficitur "
    "eget ligula. Praesent feugiat consectetur pretium. Nunc lobortis iaculis "
    "nunc, non faucibus dolor viverra eget. Nam volutpat urna ac mi lacinia, "
    "in placerat velit gravida. Integer efficitur iaculis consequat. Nulla "
    "rhoncus est massa, non faucibus dolor blandit porttitor."
    "Praesent fermentum, quam ut tincidunt dapibus, velit ipsum consequat "
    "turpis, eget tincidunt nulla nunc eget metus. Nulla a ullamcorper dolor. "
    "Ut sed mauris id magna sollicitudin gravida. Sed malesuada felis non "
    "turpis suscipit, eu lobortis nunc pellentesque. Nam vitae porttitor "
    "tortor, et iaculis tellus. In et condimentum erat. Sed in interdum ex. "
    "Sed a nisl sem. Curabitur in iaculis velit. Aliquam consequat, elit sed "
    "dignissim mollis, massa lectus placerat turpis, non porttitor arcu nunc "
    "sagittis est. Aenean et lorem sed risus pharetra rhoncus. Pellentesque "
    "dictum pharetra varius. Pellentesque eu arcu ex. Quisque condimentum, "
    "enim sed commodo faucibus, massa erat rhoncus eros, nec blandit ex purus "
    "at nisi. Vivamus hendrerit, turpis at euismod eleifend, lectus justo "
    "venenatis lorem, consequat congue leo ex quis ligula."
    "Interdum et malesuada fames ac ante ipsum primis in faucibus. Class "
    "aptent taciti sociosqu ad litora torquent per conubia nostra, per "
    "inceptos himenaeos. Pellentesque eu tortor a velit porttitor aliquet. "
    "Maecenas vitae ex at nibh aliquet efficitur non eget mi. Integer erat "
    "leo, dignissim ac lorem sit amet, commodo consequat turpis. Vivamus sed "
    "quam tristique, tristique dolor at, faucibus sem. Phasellus sodales nibh "
    "id maximus iaculis. Maecenas lacinia, nibh sodales finibus interdum, nibh "
    "dui maximus nunc, at fermentum orci urna eget mi."
    "Fusce aliquet at massa eget fringilla. Proin consequat odio ac tellus "
    "placerat dignissim eu ac ex. Vestibulum ultrices, nulla ut placerat "
    "hendrerit, sem leo efficitur risus, a cursus magna felis in leo. Aenean "
    "et vehicula nibh. Mauris eleifend, lectus quis varius efficitur, mauris "
    "arcu porttitor orci, id cursus felis diam eu turpis. Donec hendrerit nibh "
    "in urna consectetur, at gravida ipsum pharetra. Quisque ut tempus libero. "
    "Sed at auctor eros. Integer pretium consectetur nulla, vel fringilla "
    "ipsum scelerisque in. Nam ex orci, placerat ac tortor ut, convallis "
    "laoreet augue. Phasellus vulputate mi leo, vel aliquet justo tincidunt "
    "at. Proin luctus ullamcorper nibh, hendrerit iaculis tellus luctus id. In "
    "posuere dictum metus. Donec vitae turpis pellentesque, luctus nisi eu, "
    "posuere nisl. Nullam efficitur justo dui, nec aliquet velit imperdiet a."
    "Vivamus viverra dui egestas, venenatis nunc condimentum, sagittis sem. "
    "Cras eget ligula bibendum lacus pellentesque aliquet. Proin quis "
    "fringilla ipsum. In efficitur elit eget leo convallis imperdiet. Fusce "
    "porta magna in orci congue, eget lacinia orci blandit. Phasellus "
    "ultricies diam odio, quis placerat eros consequat et. Donec eu ligula "
    "luctus, maximus mi eget, pulvinar magna. Nunc quis metus leo. Etiam et "
    "pretium felis. Suspendisse sagittis diam ut interdum dapibus. Curabitur "
    "odio augue, vestibulum sit amet felis vel, cursus porta lacus. Nunc "
    "consequat neque vitae lectus commodo, eget suscipit ligula bibendum."
    "Nullam nisi ante, gravida commodo risus vitae, facilisis placerat dolor. "
    "In fermentum justo eu libero imperdiet varius. Duis ut pretium nunc, quis "
    "luctus lectus. Sed lorem diam, pulvinar vestibulum ligula vitae, suscipit "
    "vehicula mauris. Curabitur sagittis suscipit pretium. Vestibulum ante "
    "ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; "
    "Donec faucibus augue et aliquet elementum. Quisque urna metus, auctor ac "
    "orci a, faucibus tristique neque. Vivamus pharetra mattis ligula, ac "
    "scelerisque sem cursus sit amet. Pellentesque habitant morbi tristique "
    "senectus et netus et malesuada fames ac turpis egestas. Quisque et "
    "lobortis elit."
    "Integer congue odio quis augue congue ultricies. Proin luctus enim sit "
    "amet justo molestie, sit amet sagittis ex fermentum. Quisque vehicula "
    "lacinia dui eu ultrices. Proin pretium pulvinar purus at fringilla. "
    "Suspendisse arcu nulla, pellentesque viverra mattis vel, rhoncus ac "
    "ipsum. Nam condimentum felis justo, at volutpat ligula laoreet quis. "
    "Etiam rhoncus mollis varius. Nam non dolor odio. Praesent at tincidunt "
    "urna, nec efficitur felis."
    "Ut at purus iaculis urna rutrum eleifend suscipit eu lacus. Ut libero "
    "felis, lobortis sed malesuada vel, consequat vel tellus. Duis euismod "
    "velit ac erat dignissim condimentum. Integer sed tempor nulla. Mauris a "
    "ipsum congue nunc maximus ultrices. Sed sit amet rhoncus nunc, sed dictum "
    "metus. Nunc dignissim dolor et pellentesque pharetra. Aliquam blandit "
    "malesuada dignissim. Donec et libero sapien. Fusce a nisi ut ligula "
    "ultrices placerat nec id dui. Morbi feugiat magna et massa porta gravida. "
    "Pellentesque at finibus nunc. Vestibulum ornare fringilla bibendum. "
    "Mauris in blandit mi, a accumsan nunc. Suspendisse et iaculis purus."
    "Proin eleifend nisi non malesuada posuere. Donec suscipit ultrices nisl, "
    "vitae condimentum nisl consectetur hendrerit. Pellentesque aliquam in "
    "enim ut pulvinar. Sed non dui hendrerit, laoreet eros vel, porta dui. "
    "Aliquam quis dui tortor. Vestibulum auctor nibh vitae vehicula ultrices. "
    "Morbi tincidunt placerat blandit. Vestibulum dapibus felis libero, sit "
    "amet consequat purus posuere eget. Pellentesque eu pharetra turpis."
    "Vivamus dignissim porttitor libero sed feugiat. Donec porta nisi in "
    "ligula tincidunt dictum. Maecenas convallis feugiat metus, sit amet "
    "pulvinar felis mollis quis. Aenean vitae magna condimentum, interdum "
    "justo ac, ornare dui. Integer commodo tortor vitae ante maximus, nec "
    "dictum metus consectetur. Aenean in massa orci. Integer convallis, leo "
    "non posuere commodo, velit orci placerat felis, eu scelerisque neque "
    "tellus ac enim. Maecenas vestibulum lobortis augue vitae elementum. "
    "Nullam a commodo est."
    "Morbi sapien arcu, elementum consequat elementum vitae, egestas faucibus "
    "dolor. Aliquam erat volutpat. Integer semper est malesuada metus eleifend "
    "sollicitudin. Praesent elementum nulla ut augue pretium pharetra. Nulla "
    "et elit nisl. Mauris sodales dignissim metus. Proin finibus eu massa at "
    "interdum. Mauris sapien ante, egestas vitae bibendum id, egestas vitae "
    "mi. Maecenas congue ultrices ante, eu tristique dolor mattis a. "
    "Suspendisse potenti. Cras mollis nulla vitae magna pharetra, nec molestie "
    "mi luctus. Sed arcu dui, auctor vitae magna a, tristique malesuada est. "
    "Ut et vestibulum mauris. Maecenas feugiat varius mauris ac tempus."
    "Aliquam viverra dolor orci, non sollicitudin dolor tincidunt mattis. "
    "Nullam vel nulla auctor, accumsan mauris at, lacinia purus. In commodo "
    "quis magna et commodo. Curabitur in ultrices libero, iaculis faucibus "
    "dui. Donec sit amet scelerisque sapien. Vestibulum ante ipsum primis in "
    "faucibus orci luctus et ultrices posuere cubilia curae; Mauris vitae erat "
    "sem."
    "Praesent vulputate arcu id lacus sodales, non ornare mauris vestibulum. "
    "Sed id justo laoreet, hendrerit felis eget, malesuada odio. Integer "
    "venenatis augue enim, eu tincidunt magna molestie quis. Nam ac neque "
    "sodales, luctus purus sed, iaculis quam. Proin pharetra bibendum risus "
    "quis luctus. Mauris pulvinar purus semper, ultrices massa sit amet, "
    "egestas massa. In pretium lectus eros, a accumsan ipsum pharetra ac. "
    "Vestibulum diam nibh, porttitor vel nisi commodo, placerat dapibus metus. "
    "Proin eleifend odio non sapien ultricies, quis sagittis dui semper. "
    "Praesent commodo mauris ante, sit amet posuere neque maximus non. Duis a "
    "nunc sed velit bibendum porta vitae ut tortor."
    "Nunc quam risus, sollicitudin laoreet nisl vitae, ornare viverra tellus. "
    "Donec efficitur justo quis dictum blandit. Fusce egestas, mauris eu "
    "aliquam cursus, libero purus porttitor velit, at euismod libero nunc ut "
    "massa. Vivamus sed enim nec libero vestibulum egestas. Aliquam erat "
    "volutpat. Donec imperdiet finibus nunc, sed rutrum ipsum vehicula eu. Sed "
    "dapibus nisl quis ante commodo mattis. Fusce lacus eros, efficitur et "
    "ante ac, egestas dignissim nisl. Ut laoreet maximus justo vel rutrum. "
    "Nulla vel mauris condimentum, rhoncus libero in, blandit neque. Duis a "
    "cursus quam. Morbi scelerisque nunc ac hendrerit finibus. Fusce maximus "
    "nec lorem vel varius. Morbi vulputate consectetur suscipit."
    "Mauris consequat ex in dolor ullamcorper, id pretium dui placerat. "
    "Praesent imperdiet tempus feugiat. Fusce viverra quis magna sed "
    "ullamcorper. Etiam iaculis arcu purus, non cursus nunc consectetur quis. "
    "Donec tempus, nisl et molestie condimentum, massa metus elementum erat, "
    "non mollis velit ante a nunc. Proin condimentum elit quis risus dapibus "
    "aliquam. Ut ut ultricies sapien. Curabitur fermentum orci mi, eu "
    "facilisis libero commodo nec. Aliquam aliquet, lorem sit amet feugiat "
    "volutpat, felis purus tempor odio, non placerat erat orci sit amet nibh. "
    "Nam auctor mi et libero tempor scelerisque. Mauris mattis lobortis orci a "
    "faucibus. Phasellus ullamcorper eros odio, at lobortis erat malesuada "
    "quis. Mauris ultricies luctus eleifend. Mauris bibendum imperdiet "
    "commodo. Mauris vitae fermentum turpis, eget ornare nibh."
    "Pellentesque sed eros elit. Ut commodo dui in mattis sagittis. Quisque "
    "mattis, eros id consectetur mollis, justo magna euismod tellus, a rutrum "
    "dolor sapien quis mauris. In in nunc tortor. Sed porttitor non magna eu "
    "finibus. Sed in eros non sapien sodales imperdiet. Vivamus euismod "
    "consequat est commodo eleifend. Maecenas sed efficitur turpis. Donec "
    "aliquet pretium ligula. Morbi massa elit, laoreet sit amet metus in, "
    "egestas facilisis nunc. Suspendisse in interdum risus, non lobortis enim. "
    "Phasellus suscipit magna sapien. In et est felis."
    "</div>",
    NULL);

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

int
UI_run(UI uiInstance, int argc, char** argv, const void (*callback)(UI))
{
  Private uiPrivateInstance = (Private)uiInstance->private;
  appOnRunningHandler = callback;

  g_signal_connect(
    uiPrivateInstance->app, "activate", G_CALLBACK(onAppActivated), uiInstance);

  return g_application_run(G_APPLICATION(uiPrivateInstance->app), argc, argv);
}

void
UI_destroy(UI* uiInstance)
{
  Private uiPrivateInstance = (Private)((*uiInstance)->private);
  GtkApplication* app = uiPrivateInstance->app;

  g_object_unref(app);
  free(uiPrivateInstance);
  free(*uiInstance);
  *uiInstance = NULL;
}

void
UI_showDialog(const UI uiInstance, const char* title, const char* message)
{
  Private uiPrivateInstance = (Private)uiInstance->private;

  GtkWidget* dialog =
    gtk_dialog_new_with_buttons(title,
                                uiPrivateInstance->rootContainer,
                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                "OK",
                                GTK_RESPONSE_NONE,
                                NULL);
  GtkWidget* contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget* label = gtk_label_new(message);

  g_signal_connect_swapped(
    dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

  gtk_container_add(GTK_CONTAINER(contentArea), label);
  gtk_widget_show_all(dialog);
}

void (*searchButtonCallback)(char*) = NULL;

void
searchButtonOnClicked(GtkButton* button, gpointer userData)
{
  UI uiInstance = (UI)userData;
  Private uiPrivateInstance = (Private)uiInstance->private;

  char* searchQuery =
    gtk_entry_get_text(GTK_ENTRY(uiPrivateInstance->searchInput));

  if (searchButtonCallback)
    searchButtonCallback(searchQuery);
}

void
UI_onSearchButtonClicked(const UI uiInstance, const void (*handler)(char*))
{
  Private uiPrivateInstance = (Private)uiInstance->private;
  searchButtonCallback = handler;

  g_signal_connect(uiPrivateInstance->searchButton,
                   "clicked",
                   G_CALLBACK(searchButtonOnClicked),
                   uiInstance);
}

void (*treeViewCallback)(char*);

void
treeSelectionOnChanged(GtkTreeSelection* selection, gpointer userData)
{
  UI uiInstance = (UI)userData;
  Private uiPrivateInstance = (Private)uiInstance->private;

  GtkTreeModel* model =
    gtk_tree_view_get_model(GTK_TREE_VIEW(uiPrivateInstance->listView));
  GtkTreeIter iter;
  char* word;

  gtk_tree_selection_get_selected(selection, &model, &iter);
  gtk_tree_model_get(model, &iter, 0, &word, -1);

  treeViewCallback(word);
}

void
UI_onListViewItemClicked(const UI uiInstance, const void (*handler)(char*))
{
  Private uiPrivateInstance = (Private)uiInstance->private;

  treeViewCallback = handler;
  GtkTreeSelection* selection =
    gtk_tree_view_get_selection(GTK_TREE_VIEW(uiPrivateInstance->listView));

  g_signal_connect(
    selection, "changed", G_CALLBACK(treeSelectionOnChanged), uiInstance);
}
