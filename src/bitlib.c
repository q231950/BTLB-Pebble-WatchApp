/*
 * bitlib.c
 */

#include <pebble.h>
#include "bookmarkswindow.h"

#define NUM_FIRST_MENU_ITEMS 2

static Window *s_main_window;;
static MenuLayer *s_menu_layer;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
            return 2;
        default:
            return 0;
    }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    static char title[32];
    
    switch (cell_index->section) {
        case 0:
            switch (cell_index->row) {
                case 0:
                    snprintf(title, sizeof(title), "bookmarks");
                    break;
                case 1: {
                    snprintf(title, sizeof(title), "loans");
                    break;
                }
                default:
                    snprintf(title, sizeof(title), " ");
                    break;
                }
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
            APP_LOG(APP_LOG_LEVEL_ERROR, "Sele ct bookmarks");
            showBookmarks();
            break;
        default:
            break;
    }    
}

// Window

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
}

static void main_window_unload(Window *window) {
    menu_layer_destroy(s_menu_layer);
}

// Initialisation

static void init(void) {
    // Create main Window
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    window_stack_push(s_main_window, true);
}

static void deinit(void) {
    // Destroy main Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
