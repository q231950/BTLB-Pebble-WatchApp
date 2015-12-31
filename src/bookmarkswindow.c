#include <Pebble.h>
#include "bookmarkswindow.h"

static Window *window;

static void initialiseUI() {
  window = window_create();

#ifndef PBL_SDK_3
  window_set_fullscreen(window, true);
#endif
}

void showBookmarks(void) {
  initialiseUI();

  window_set_window_handlers(window, (WindowHandlers) {
      .load = NULL,
      .unload = NULL,
      });
  window_stack_push(window, true);
}
