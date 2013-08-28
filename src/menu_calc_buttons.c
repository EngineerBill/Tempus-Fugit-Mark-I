/* -------------------------------------------------------------------------------------- *\

                      menu_calc_buttons.c

	Implements Tempus Fugit Calc function Menu buttons select option

	Called:		long select button

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/19/13
   ----------------------------------------------------------------

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"		// global declares
#include "menu_calc.h"			// local module declares
#include "menu_calc_buttons.h"	//
#include "feature_calc.h"		//



static int pending_buttons;		// used to track user choice

static Window window;			// Local Action Bar Window

static ActionBarLayer action_bar;	// Needed Window layers
static TextLayer button_text_layer;	// 

static char *people_ptr = "People";	// Display strings
static char *time_ptr = "Time";		//
static char *wages_ptr = "Wages";
// If the following string appears it means
// that the pending_buttons variable has been
// gorf'ed and we've lost the bubble...
// Theoretically this can't happen, but
// things like writing past the end of a buffer
// can do some strange thigngs...
static char * error_ptr = "ERROR: pending_buttons!";

static void window_load();
static void window_unload();
static void clicked_up();
static void clicked_select();
static void clicked_down();
static void click_config_provider();

// --------------------------------------------------------
//			menu_calc_button_select()
// --------------------------------------------------------
void menu_calc_buttons_show_page() {

    window_stack_push(&window, true);

}  // menu_calc_button_select()

// --------------------------------------------------------
//			menu_calc_button_init()
// --------------------------------------------------------
void menu_calc_buttons_select(){
	
//GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
	
    window_init(&window, "Calc Menu - Buttons");
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

	pending_buttons = feature_calc_buttons_get();
	
char *ptr = people_ptr;

	switch(pending_buttons) {
	case 0:	ptr = people_ptr;
			break;
	case 1:	ptr = time_ptr;
			break;
	case 2:	ptr = wages_ptr;
			break;
	default: ptr = error_ptr;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);
	text_layer_set_background_color(&button_text_layer, GColorWhite);
	text_layer_set_text_color(&button_text_layer, GColorBlack);
	text_layer_set_font(&button_text_layer, norm24);
	layer_add_child(&window.layer, &button_text_layer.layer);

	window_stack_push(&window, true);


}  // menu_calc_buttons_init()

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
	
	ptr = people_ptr;

	if(++pending_buttons > 2) {
		pending_buttons = 0;
	}

	switch(pending_buttons) {
	case 0:	ptr = people_ptr;
			break;
	case 1:	ptr = time_ptr;
			break;
	case 2:	ptr = wages_ptr;
			break;		
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {
	
	feature_calc_buttons_set(pending_buttons);
	menu_calc_cleanup();	// Remove menu window so we 
							//pop back to Stopwatch function, not menu

	window_stack_pop(true);
	
	
}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {
char *ptr;
	
	ptr = people_ptr;

	if(--pending_buttons == -1) {
		pending_buttons = 2;
	}

	switch(pending_buttons) {
	case 0:	ptr = people_ptr;
			break;
	case 1:	ptr = time_ptr;
			break;
	case 2:	ptr = wages_ptr;
			break;			
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_down()
