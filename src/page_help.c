/* -------------------------------------------------------------------------------------- *\

                      page_help.c

	Implements Tempus Fugit context-sensitive Help Window

	Called from :
					- Main Program menu
					- Calculator function menu
					- Stopwatch function menu
					- Countdown Timer function menu

	To invoke, call the public function page_help_show_page(CONTEXT)
	 the values for CONTEXT are defined in TempusFugit.h and may
	 include:
				- PROGRAM
				- CALCULATOR_ABOUT
				- CALCULATOR_HELP
				- #define STOPWATCH_ABOUT
				- STOPWATCH_HELP
				- TIMER_ABOUT
				- TIMER_HELP

TODO:

The text to be displayed is currently declared in an array in this module.
Ideally this should be moves to a header file for easier access.
	
History:
		Derived from - PebbleSDK-1-12:

	~/Pebble/demos/feature_scroll_layer/src/feature_scroll_layer.c

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/18/13
	Last Update:		8/27/13
   ----------------------------------------------------------------



\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"			// global declares
#include "page_help.h"				// local declares

// ---------------------------
//    Edit Help Text here
// ---------------------------

char program_scroll_text[] = "Tempus Fugit:\nA light-hearted but hopefully useful App with Meeting Cost Calculator, Stopwatch, Countdown Timer & Analog Clock.\nSELECT brings up each function's control Menu.\nBUTTONS allows you to control UP/DOWN buttons for each function (see function HELP for more details).\n----\n----\n";

char calc_about_scroll_text[] = "Meeting Calculator:\nStatic or real time measurement of the cost of a meeting, based upon user-specified Number of Attendees, Estimated Average Salary and Elapsed Time.";
//char calc_help_scroll_text[] = "START/STOP toggles the calculation timer (Note: you can add or remove time with the UP/DOWN buttons while the timer is running).\nRESET stops the timer, clears count and restores defaults.\nCLEAR sets the timer count to zero but does not stop it.\nBUTTONS allows you to specify which parameter is controlled by UP/DOWN buttons (options are PEOPLE,WAGES or TIME).\n----------\n";

char stop_about_scroll_text[] = "Stopwatch:\nTraditional elapsed time function, with Alert capability (Buzzer vibrates once every ALERT minutes, if set).";
//char stop_help_scroll_text[] = "START/STOP toggles stopwatch timer (Note: you can still add or remove time with the UP/DOWN buttons while the timer is running).\nRESET stops the timer, clears count and restores defaults.\nCLEAR sets the timer count to zero but does not stop it.\nBUTTONS allows you to specify which parameter is controlled by UP/DOWN buttons (options are COUNT or TIME).\n----------\n";

char timer_about_scroll_text[] = "Countdown Timer:\nTraditional interval timer function, with Alert capability (Buzzer vibrates twice every ALERT minutes, if set).";
//char timer_help_scroll_text[] = "START/STOP toggles countdown timer (Note: you can still add or remove time with the UP/DOWN buttons while the timer is running).\nRESET stops the timer and clears the count.\nCLEAR sets the timer count to zero but does not stop it.\nBUTTONS allows you to specify which parameter is controlled by UP/DOWN buttons (available choices are COUNT or TIME).\n----------\n";
char help_scroll_text[] = "START/STOP toggles timer (Note: you can still add or remove time with the UP/DOWN buttons while timer is running).\nRESET stops timer and clears the count.\nCLEAR sets timer count to zero but does not stop it.\nBUTTONS allows you to specify which parameter is controlled by UP/DOWN buttons.\n-----\n";

//  Short form test here for debugging
/*char program_scroll_text[] = "Tempus Fugit:\n";

char calc_about_scroll_text[] = "Meeting Calculator:";
char calc_help_scroll_text[] = "START/STOP";

char stop_about_scroll_text[] = "Stopwatch:";
char stop_help_scroll_text[] = "START/STOP";

char timer_about_scroll_text[] = "Countdown Timer:";
char timer_help_scroll_text[] = "START/STOP";
*/

// Needed Window Layer and other defines
//static TextLayer help_layer;	// info about each feature
//} help_data;

static ScrollLayer scroll_layer;	// here's where the magic happens
static TextLayer text_layer;		// only one text layer for now

static const int vert_scroll_text_padding = 4;


static Window help_window;

static void handle_appear();
static void handle_disappear();
static void handle_load();
static void handle_unload();

// --------------------------------------------------------
//			page_help_show_page()
//
//   public function to show context sensitive Help Page
//   (Called with flag indicting which Help page to show.
//     these flags are defined in "TempusFugit.h Add flag
//         there, plus another text array above and 
//          another case statement here to extend)
//
// --------------------------------------------------------
void page_help_show_page(int page){

	scroll_layer_set_content_offset(&scroll_layer, GPointZero, false);

	switch (page) {
	case PROGRAM:
				text_layer_set_text(&text_layer, program_scroll_text);
				break;

	case CALCULATOR_HELP:
				text_layer_set_text(&text_layer, help_scroll_text);
				break;

	case CALCULATOR_ABOUT:
				text_layer_set_text(&text_layer, calc_about_scroll_text);
				break;

	case STOPWATCH_ABOUT:
				text_layer_set_text(&text_layer, stop_about_scroll_text);
				break;

	case STOPWATCH_HELP:
				text_layer_set_text(&text_layer, help_scroll_text);
				break;

	case TIMER_ABOUT:
				text_layer_set_text(&text_layer, timer_about_scroll_text);	//
				break;
	case TIMER_HELP:
				text_layer_set_text(&text_layer, help_scroll_text);	//
				break;
	}
	
	window_stack_push(&help_window, true);

}  // page_help_show_page()


// --------------------------------------------------------------------
//            Window Callbacks
// --------------------------------------------------------------------
// --------------------------------------------------------
//		handle_appear()
//
//   called for housekeeping when window appears.
// --------------------------------------------------------
static void handle_appear() {

	
}  // handle_appear()

// --------------------------------------------------------
//		handle_disappear()
//
//   perform cleanup when window is hidden.
// --------------------------------------------------------
static void handle_disappear() {

	
}  // handle_disappear()

static void handle_load() {

}  // handle_load()

static void handle_unload() {

}  // handle_unload()


// --------------------------------------------------------
//		page_help_init()
//
//  Called by TempusFugit.c when program is first init'ed
// --------------------------------------------------------
void page_help_init() {
	window_init(&help_window, "Tempus Fugit Help");
	window_set_window_handlers(&help_window, (WindowHandlers) {
		.appear = handle_appear,
		.disappear = handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
  });

	window_set_background_color(&help_window, GColorBlack);

// set up text bounds beyond window bounds for ScrollLater capability
const GRect max_text_bounds = GRect(0, 0, 144, 2000);

// Initialize the scroll layer
	scroll_layer_init(&scroll_layer, help_window.layer.bounds);

// Now bind the scroll layer to the window so that up and down buttons
// map to scrolling. You can choose to set up and use callbacks using
// scroll_layer_set_callbacks to add or override interactivity
	scroll_layer_set_click_config_onto_window(&scroll_layer, &help_window);

// Set the scroll layer's max size
	scroll_layer_set_content_size(&scroll_layer, max_text_bounds.size);


// Now initialize the text layer
	text_layer_init(&text_layer, max_text_bounds);
	text_layer_set_text(&text_layer, help_scroll_text);	// NOTE: we start with largest text to setmax_size below...
	text_layer_set_background_color(&text_layer, GColorBlack);	// In operation, each call to this module will
	text_layer_set_text_color(&text_layer, GColorWhite);		// specify text to show

//	text_layer_set_font(&text_layer, norm14);
// Change the font to something large enough to be legible...
// This uses a default system font; you can check out all the system fonts
// in the system include file "pebble_fonts.h"" or you can take a look at
// the sample program "feature_custom_font" " to see how to add your own...
	text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

// Trim text layer and scroll content to fit text box
	GSize max_size = text_layer_get_max_used_size(app_get_current_graphics_context(), &text_layer);
	text_layer_set_size(&text_layer, max_size);
	scroll_layer_set_content_size(&scroll_layer, GSize(144, max_size.h + vert_scroll_text_padding));
	scroll_layer_set_content_offset(&scroll_layer, GPointZero, false);

// Add the layers for display
	scroll_layer_add_child(&scroll_layer, &text_layer.layer);

// The inverter layer will highlight some text
//	inverter_layer_init(&inverter_layer, GRect(0, 28, window.layer.frame.size.w, 28));
//	scroll_layer_add_child(&scroll_layer, &inverter_layer.layer);

	layer_add_child(&help_window.layer, &scroll_layer.layer);

}  // page_help_init()

