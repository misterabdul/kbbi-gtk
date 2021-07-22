#include <stdlib.h>
#include <string.h>

#include <libkbbi/libkbbi.h>

#include "ui.h"

#define APP_ID "io.github.misterabdul.kbbi-gtk"
#define WINDOW_TITLE "KBBI Offline"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

KBBI_Results results = NULL;

/**
 * Result words array.
 */
char** resultWords = NULL;

/**
 * Result word meanings array.
 */
char** resultMeans = NULL;

/**
 * On error dialog responded function handler.
 *
 * @param[in] ui
 */
void
onDialogResponded(UI ui)
{
  UI_stop(ui);
}

/**
 * On search button clicked function handler.
 *
 * @param[in] ui
 * @param[in] query
 */
void
onSearchButtonClicked(UI ui, char* query)
{
  if (strlen(query) > 0) {
    UI_setWebViewContent(ui, "<div></div>");

    if (!results)
      results = KBBI_resultInit();

    int resultSize = 0;
    int found = KBBI_search(&results, &resultSize, query, strlen(query));

    if (found && results) {
      char** words = malloc(resultSize * sizeof(char*));
      char** means = malloc(resultSize * sizeof(char*));

      KBBI_Results resultNode = results;
      for (int i = 0; i < resultSize; i++) {
        if (!resultNode)
          break;

        if (resultNode->katakunci)
          words[i] = resultNode->katakunci;

        if (resultNode->artikata)
          means[i] = resultNode->artikata;

        resultNode = resultNode->next;
      }

      UI_setListViewItems(ui, words, resultSize);

      if (resultWords)
        free(resultWords);
      resultWords = words;

      if (resultMeans)
        free(resultMeans);
      resultMeans = means;
    }
  }
}

/**
 * On ListView's item clicked function handler.
 *
 * @param[in] ui
 * @param[in] word
 * @param[in] index
 */
void
onListViewItemClicked(UI ui, char* word, int index)
{
  UI_setWebViewContent(ui, resultMeans[index]);
}

/**
 * On app running function handler.
 *
 * @param[in] ui
 */
void
onAppRunning(UI ui)
{
  UI_onListViewItemClicked(ui, onListViewItemClicked);
  UI_onSearchButtonClicked(ui, onSearchButtonClicked);
}

/**
 * Application main entry function.
 *
 * @param[in] argc
 * @param[in] argv
 * @return
 */
int
main(int argc, char** argv)
{
  UI ui = UI_init(APP_ID, WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

  int status = UI_run(ui, argc, argv, onAppRunning);

  if (ui)
    UI_destroy(&ui);

  if (results)
    KBBI_resultFree(results);
  if (resultWords)
    free(resultWords);
  if (resultMeans)
    free(resultMeans);

  return status;
}
