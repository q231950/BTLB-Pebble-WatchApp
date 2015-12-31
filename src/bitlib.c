/*
 * bitlib.c
 */

#include <pebble.h>

#define KEY_COUNT 5

static Window *s_main_window;;
static MenuLayer *s_menu_layer;
static AppSync s_sync;
static uint8_t s_sync_buffer[32];

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
            return NUM_FIRST_MENU_ITEMS;
        case 1:
            return NUM_SECOND_MENU_ITEMS;
        default:
            return 0;
    }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch (cell_index->section) {
        case 0:
            switch (cell_index->row) {
                default: {
                    menu_cell_basic_draw(ctx, cell_layer, "Basic Item", "With a subtitle", NULL);
                    break;
                }
            }
            break;
        case 1:
            switch (cell_index->row) {
                default:
                {
#ifdef PBL_RECT
                    menu_cell_title_draw(ctx, cell_layer, "Final Item");
#else
                    GSize size = layer_get_frame(cell_layer).size;
                    graphics_draw_text(ctx, "Final Item", fonts_get_system_font(FONT_KEY_GOTHIC_28),
                                       GRect(0, 0, size.w, size.h), GTextOverflowModeTrailingEllipsis,
                                       GTextAlignmentCenter, NULL);
#endif
                    break;
                }
            }
    }
}

#ifdef PBL_ROUND
static int16_t get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    if (menu_layer_is_index_selected(menu_layer, cell_index)) {
        switch (cell_index->row) {
            case 0:
                return MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT;
                break;
            default:
                return MENU_CELL_ROUND_FOCUSED_TALL_CELL_HEIGHT;
        }
    } else {
        return MENU_CELL_ROUND_UNFOCUSED_SHORT_CELL_HEIGHT;
    }
}
#endif

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    // Use the row to specify which item will receive the select action
    switch (cell_index->row) {
        case 1:
            break;
    }
}

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    static char s_bookmarks_buffer[32];
    snprintf(s_bookmarks_buffer, sizeof(s_count_buffer), "Count: %d", (int)new_tuple->value->int32);
    //    text_layer_set_text(s_output_layer, s_count_buffer);
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    // An error occured!
    APP_LOG(APP_LOG_LEVEL_ERROR, "sync error!");
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
    
    // Setup AppSync
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
    // Setup initial values
    Tuplet initial_values[] = {
        TupletInteger(KEY_COUNT, 0),
    };
    
    // Begin using AppSync
    app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);
}

static void deinit(void) {
    // Destroy main Window
    window_destroy(s_main_window);
    
    // Finish using AppSync
    app_sync_deinit(&s_sync);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
