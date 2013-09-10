#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "feature_simplicity.h"		// added during conversion to Tempus feature


// ------------------------------------------------------------
//  original Watchface UUID
// ------------------------------------------------------------
//#define MY_UUID {0xA4, 0x1B, 0xB0, 0xE2, 0xD2, 0x62, 0x4E, 0xA6, 0xAA, 0x30, 0xED, 0xBE, 0x01, 0xE3, 0x8A, 0x02}
//PBL_APP_INFO(MY_UUID, "Simplicity", "Pebble Technology", 3, 0 /* App version */, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

	
// ------------------------------------------------------------
//    local declares
// ------------------------------------------------------------

static Window window;

static TextLayer text_date_layer;
static TextLayer text_time_layer;

static Layer line_layer;

static int tick_count = 0;

// ------------------------------------------------------------
//     public functions
// ------------------------------------------------------------
// --------------------------------------------------
//  feature_simplicity_show_window()
//
//       Called from main program menu
//         to show feature window
// --------------------------------------------------
void feature_simplicity_show_window(){

  window_stack_push(&window, true /* Animated */);

}  // feature_simplicity_show_window()


// ------------------------------------------------------------
//     original watchface functions
// ------------------------------------------------------------ 
static void line_layer_update_callback(Layer *me, GContext* ctx) {

  graphics_context_set_stroke_color(ctx, GColorWhite);

  graphics_draw_line(ctx, GPoint(8, 97), GPoint(131, 97));
  graphics_draw_line(ctx, GPoint(8, 98), GPoint(131, 98));

}


void feature_simplicity_tick(AppContextRef ctx, PebbleTickEvent *t) {
//void feature_simplicity_tick() {

  // Need to be static because they're used by the system later.
static char time_text[] = "00:00";
static char date_text[] = "Xxxxxxxxx 00";

char *time_format;

	if(++tick_count%60) {

  // TODO: Only update the date when it's changed.
		string_format_time(date_text, sizeof(date_text), "%B %e", t->tick_time);
		text_layer_set_text(&text_date_layer, date_text);

		if (clock_is_24h_style()) {
			time_format = "%R";
		}
		else {
			time_format = "%I:%M";
		}

		string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

// Kludge to handle lack of non-padded hour format string
// for twelve hour clock.
		if (!clock_is_24h_style() && (time_text[0] == '0')) {
			memmove(time_text, &time_text[1], sizeof(time_text) - 1);
		}
		text_layer_set_text(&text_time_layer, time_text);
	}
}  // feature_simplicifty_tick()

// --------------------------------------------------------------------------
//          click provider section
// --------------------------------------------------------------------------

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_down()

// --------------------------------------------------------
//			clicked_config_provider()
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
     config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
   config[BUTTON_ID_DOWN]->click.handler = clicked_down;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}
// ---------------------------------------------------------------------
//  init()/deinit() routines adapted during conversion
//
//       Called from TempusFugit.c
//
// ---------------------------------------------------------------------
void feature_simplicity_init(AppContextRef ctx) {

  window_init(&window, "Simplicity");
 //  ------------------------------------------------------
//   Set up window click handlers (Added for Tempus Fugit)
 //  ------------------------------------------------------
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);

  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);

  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  layer_set_frame(&text_date_layer.layer, GRect(8, 68, 144-8, 168-68));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_date_layer.layer);


  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  layer_set_frame(&text_time_layer.layer, GRect(7, 92, 144-7, 168-92));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(&window.layer, &text_time_layer.layer);


  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);


  // TODO: Update display here to avoid blank display on launch?
}


void handle_simplicity_deinit() {


}  // handle_simplicity_deinit()