#ifndef _UI_H
#define _UI_H

/**
 * The UI struct.
 */
typedef struct _ui
{
  void* private;
} * UI;

/**
 * Initialize the UI.
 *
 * @param[in] appId   The application ID.
 * @param[in] title   The application title.
 * @param[in] width   The window's initial width.
 * @param[in] height  The window's initial height.
 * @return            The UI instance.
 */
UI
UI_init(const char* appId,
        const char* title,
        const int width,
        const int height);

/**
 * Run the UI.
 *
 * @param[in] ui            The UI instance.
 * @param[in] argc          Number of command parameter's string.
 * @param[in] argv          Strings of command parameters.
 * @param[in] onAppRunning  Handler function after application's window displayed.
 * @return                  Return status after application terminated.
 */
int
UI_run(UI ui, int argc, char** argv, const void (*onAppRunning)(UI));

/**
 * Stop the running UI.
 *
 * @param[in]  ui The UI instance.
 */
void
UI_stop(UI ui);

/**
 * Destroy the UI instance.
 *
 * @param[out]  ui  The UI instance.
 */
void
UI_destroy(UI* ui);

/**
 * Show alert dialog.
 *
 * @param[in] ui              The UI instance.
 * @param[in] title           The dialog's title.
 * @param[in] message         The dialog's message.
 * @param[in] responseHandler The dialog's response handler function.
 */
void
UI_showDialog(const UI ui,
              const char* title,
              const char* message,
              const void (*responseHandler)(UI));

/**
 * Set handler when search button clicked.
 *
 * @param[in] ui      The UI instance.
 * @param[in] handler The handler function.
 */
void
UI_onSearchButtonClicked(const UI ui, const void (*handler)(UI, char*));

/**
 * Set the ListView items.
 *
 * @param[in] ui        The UI instance.
 * @param[in] items     The items array.
 * @param[in] itemSize  The items array's size.
 */
void
UI_setListViewItems(const UI ui, const char* items[], const int itemSize);

/**
 * Set handler for ListView item clicked.
 *
 * @param[in] ui      The UI instance.
 * @param[in] handler The handler function.
 */
void
UI_onListViewItemClicked(const UI ui, const void (*handler)(UI, char*, int));

/**
 * Set the content for WebView.
 *
 * @param[in] ui    The UI instance.
 * @param[in] html  The HTML string.
 */
void
UI_setWebViewContent(const UI ui, const char* html);

#endif
