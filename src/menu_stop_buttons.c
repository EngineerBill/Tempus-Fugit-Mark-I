/* -------------------------------------------------------------------------------------- *\

                      menu_stop_buttons.c

	Implements Tempus Fugit Stopwatch function Menu buttons select option

	Called:		menu_stop() Buttons menu options
	
	Note:	This is essentially identical to the menu_timer_buttons.c
			module. See that file for detailed comments.
			("So why are they separate?" I hear you cry - By keeping
			each module stand-alone they are hopefully easier to follow
			and/or modify/coopt for other projects. If you need to 
			recover space you can consider merging the multiple menu
			routines and/or the multiple Action Bar routines to save space...

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/26/13
	Last Update:		8/26/13
   ----------------------------------------------------------------

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"			// global declares
#include "menu_stop.h"				// local module declares
#include "menu_stop_buttons.h"		//
#include "feature_stop.h"			//



static int pending_buttons;

static Window window;

static ActionBarLayer action_bar;
static TextLayer button_text_layer;

static char *count_ptr = "Count";
static char *alert_ptr = "Alert";
static char *error_ptr = "ERROR: pending_buttons!";

static void window_load();
static void window_unload();
static void clicked_up();
static void clicked_select();
static void clicked_down();
static void click_config_provider();

// --------------------------------------------------------
//			menu_stop_button_select()
// --------------------------------------------------------
void menu_stop_buttons_show_page() {

    window_stack_push(&window, true);

}  // menu_stop_button_select()

// --------------------------------------------------------
//			menu_stop_button_init()
// --------------------------------------------------------
void menu_stop_buttons_select(){
	
//GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
	
    window_init(&window, "Stop Menu - Buttons");
    window_set_window_handlers(&window, (WindowHandlers){
        .unload = window_unload,
        .load = window_load,
    });

// set up action bar
    action_bar_layer_init(&action_bar);
    action_bar_layer_add_to_window(&action_bar, &window);
    action_bar_layer_set_click_config_provider(&action_bar, click_config_provider);

    action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &tf_icon_container[0].bmp);
    action_bar_layer_set_icon(&action_bar, BUTTON_ID_SELECT, &tf_icon_container[1].bmp);
    action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &tf_icon_container[2].bmp);
	
	// set up text layer
	text_layer_init(&button_text_layer, GRect (35, 64, 55, 28));

	pending_buttons = feature_stop_buttons_get();
	
char *ptr = count_ptr;

	switch(pending_buttons) {
	case 0:	ptr = count_ptr;
			break;
	case 1:	ptr = alert_ptr;
			break;
	default: ptr = error_ptr;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);
	text_layer_set_background_color(&button_text_layer, GColorWhite);
	text_layer_set_text_color(&button_text_layer, GColorBlack);
	text_layer_set_font(&button_text_layer, norm24);
	layer_add_child(&window.layer, &button_text_layer.layer);

	window_stack_push(&window, true);


}  // menu_stop_buttons_init()

// --------------------------------------------------------
//			window_load()
// --------------------------------------------------------
void window_load(){

}  // window_load()

// --------------------------------------------------------
//			window_unload()
// --------------------------------------------------------
void window_unload(){

}  // window_unload()

static void click_config_provider(ClickConfig **config, void* context) {
    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {

char *ptr;
	
	ptr = count_ptr;

	if(++pending_buttons > 1) {
		pending_buttons = 0;
	}

	switch(pending_buttons) {
	case 0:	ptr = count_ptr;
			break;
	case 1:	ptr = alert_ptr;
			break;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {
	
	feature_stop_buttons_set(pending_buttons);
	menu_stop_cleanup();	// Remove menu window so we 
							//pop back to Stopwatch function, not menu

	window_stack_pop(true);
	
	
}  // clicked_select()

// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {
char *ptr = count_ptr;
	
	if(--pending_buttons == -1) {
		pending_buttons = 1;
	}

	switch(pending_buttons) {
	case 0:	ptr = count_ptr;
			break;
	case 1:	ptr = alert_ptr;
			break;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_down()
