/* -------------------------------------------------------------------------------------- *\

                      menu_timer_buttons.c

	Implements Tempus Fugit Timer function Action Bar menu. 
	
	Menu buttons select option.
    Creations Action Bar layer to allow user to select either COUNT or ALERT.
	This in turn determines whether the UP/DOWN buttons in the Timer function
	increment/decrement the elapsed time count or the ALERT setting.
	
	Called by:		Timer function menu 

	Theory of Operation:
	Calling: The function "menu_timer_buttons_show_page()" is invoked from
	menu_timer.c module. This schedules the button menu Window load.
	
	Return: When user makes a selection, the callback "clicked_select()" is called.
	This removes the calling menu window from the stack and then pops itself.
	This results ia the user returns seemlessly to the main Timer function Window.
	
	
	Notes:
	
	1)	This is basically identical to the companion function "menu_stop_buttons.c".
		It would be a trivial exercise to pass in a flag to determine the caller.
		This would reduce code size but make it more difficult to follow so
		we're staying with two copies for now.

	----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/27/13
	Last Update:		8/27/13
   ----------------------------------------------------------------

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"			// global declares
#include "menu_timer.h"				// local module declares
#include "menu_timer_buttons.h"		//
#include "feature_timer.h"			//



static int pending_buttons;			// used to track user choice

static Window window;				// Local Action Bar Window

static ActionBarLayer action_bar;	// Required Menu layers
static TextLayer button_text_layer;	//

static char *count_ptr = "Count";	// Displayed text strings
static char *alert_ptr = "Alert";

// if the following text appears on the menu it
// means our pending_button variable has a bad value.
// in theory this can't happen....
static char *error_ptr = "ERROR: string pointer out of range!";

// The following are generic private function prototypes
// used in each Window module. By declaring them static we
// can reuse the names without conflicts at link time.
// Note: you do not have to use these, they're placeholders.
static void window_load();
static void window_unload();
static void clicked_up();
static void clicked_select();
static void clicked_down();
static void click_config_provider();

// ----------------------------------------------------------------
//          public module functions
// ----------------------------------------------------------------
// ----------------------------------------------
//			menu_timer_button_select()
// ----------------------------------------------
void menu_timer_buttons_show_page() {

    window_stack_push(&window, true);

}  // menu_timer_button_select()

// ----------------------------------------------------------------
//          private module functions
// ----------------------------------------------------------------
// ----------------------------------------------
//			window_load()

// ----------------------------------------------
void window_load(){

}  // window_load()


// ----------------------------------------------
//			window_unload()
// ----------------------------------------------
void window_unload(){

}  // window_unload()

// ----------------------------------------------
//			click_config_provider()
//
//  Used to configure button callback functions
// ----------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// ----------------------------------------------
//			clicked_up()
//
//  cycles up through available menu strings and
// sets them to display.
// ----------------------------------------------
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



// ----------------------------------------------
//			clicked_select()
//
//  When user hits SELECT button, this callback
// first calls the Timer module routine
// feature_timer_buttons_set() to save the 
// selected option, then removes the calling menu
// Window from the stack by calling the function
// menu_timer_cleanuu(). This provides the user
// a seemless return to the Timer function.
// ----------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {
	
	feature_timer_buttons_set(pending_buttons);
	menu_timer_cleanup();	// Remove menu window so we 
							//pop back to Timer function
							// not the calling menu

	window_stack_pop(true);
	
	
}  // clicked_select()

// ----------------------------------------------
//			clicked_down()
//
//  cycles down through available menu strings
// and sets them to display.
// ----------------------------------------------
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
// --------------------------------------------------------
//			menu_timer_button_init()
//
// Called by the Timer module main menu menu_timer.c
// when the corresponding menu item is selected.
// This module creates the required Window, configures
// the button click handlers and populates the Window
// with the needed ActionBar and Text Layers. It thens
// calls PebblOS to push the new Window onto the stack. 
// --------------------------------------------------------
void menu_timer_buttons_select(){
	
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

	pending_buttons = feature_timer_buttons_get();
	
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


}  // menu_timer_buttons_init()

