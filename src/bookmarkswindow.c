#include <Pebble.h>
#include "bookmarkswindow.h"
#include "serializer.h"

#define KEY_COUNT 5

static Window *window;
static AppSync s_sync;
static uint8_t s_sync_buffer[32];
static MenuLayer *s_menu_layer;
static char ** bookmarks;

// Menu

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0: {
            int numItems = sizeof(bookmarks);
            return numItems;
        }
        default:
            return 0;
    }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    const char *title = bookmarks[cell_index->row];
    switch (cell_index->section) {
        case 0:
            title = bookmarks[cell_index->row];
            break;
    }
    
#ifdef PBL_RECT
    menu_cell_title_draw(ctx, cell_layer, title);
#else
    GSize size = layer_get_frame(cell_layer).size;
    graphics_draw_text(ctx, title, fonts_get_system_font(FONT_KEY_GOTHIC_28),
                       GRect(0, 0, size.w, MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT), GTextOverflowModeTrailingEllipsis,
                       GTextAlignmentCenter, NULL);
#endif
    
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    switch (cell_index->row) {
        case 0:
            break;
        default:
            break;
    }
}

// AppSync

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    char myString[] = "these|are|entries|yes";
    bookmarks = arrayFromString(myString);
    
    int numItems = sizeof(bookmarks);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", numItems);
    
    for (int i = 0; i < numItems; ++i) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "res[] = %s\n", bookmarks[i]);
    }
    
    menu_layer_reload_data(s_menu_layer);
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    // An error occured!
    APP_LOG(APP_LOG_LEVEL_ERROR, "sync error!");
}

// Window

static void onWindowReady() {
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
    Tuplet initial_values[] = {
        TupletInteger(KEY_COUNT, 0),
    };
    
    app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer);
    
    // Create the menu layer
    s_menu_layer = menu_layer_create(window_bounds);
    menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });
    
    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(s_menu_layer, window);
    
    layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
    
    onWindowReady();
}

static void main_window_unload(Window *window) {
    menu_layer_destroy(s_menu_layer);
}


static void initialiseUI() {
    window = window_create();
    
#ifndef PBL_SDK_3
    window_set_fullscreen(window, true);
#endif
}

void showBookmarks(void) {
    initialiseUI();
    
    window_set_window_handlers(window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    
    window_stack_push(window, true);
}
