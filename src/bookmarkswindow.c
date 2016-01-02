#include <Pebble.h>
#include "bookmarkswindow.h"
#include "serializer.h"

#define KEY_COUNT 5

static Window *window;
static AppSync s_sync;
static uint8_t s_sync_buffer[32];
static char ** bookmarks;

// AppSync

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "sync_changed_handler called");
    char myString[] = "these|are|entries|yes";
    bookmarks = arrayFromString(myString);
    
    int numItems = sizeof(bookmarks);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", numItems);
    
    for (int i = 0; i < numItems; ++i) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "res[] = %s\n", bookmarks[i]);
    }
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    // An error occured!
    APP_LOG(APP_LOG_LEVEL_ERROR, "sync error!");
}


static void initialiseUI() {
    window = window_create();
    
#ifndef PBL_SDK_3
    window_set_fullscreen(window, true);
#endif
    
    window_stack_push(window, true);
}

void showBookmarks(void) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Show bookmarks");
    
    initialiseUI();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = NULL,
        .unload = NULL,
    });
    
    
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
    Tuplet initial_values[] = {
        TupletInteger(KEY_COUNT, 0),
    };
    
    app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);
}
