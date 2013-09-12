/* -------------------------------------------------------------------------------------- *\

                      page_start.c

	Implements Tempus Fugit opening animated splash page

	Called:		from TempusFugit:handle_init()

Theory of Operation:

	The handle_init() routine in TempusFugit.c program
	initiation module calls this module's page_start_init() 
	function to create the appropriate Window structure and
	set up the needed graphics layers. The TempusFugit.c
	program entry point pbl_main() sets up a handle_timer()
	function to process timer ticks and the handle_init()
	function schedules an inital timer tick by calling
	page_start-splash_start() to begin animation.
	
	As timer ticks are received in TempusFugit:handle_timer(),
	they are despatched to this module's page_start_tick()
	function. This tracks progress with flags and changes timer
	duration depending on state (Icons are animated quickly, text
	more slowly by varying the calls to the system timer function
	app_timer_send_event() after each tick. Feel free to experiment
	with this!)
	
	Note that the needed splash page icons are loaded in the
	TempusFugit.c handle_init() function (and *must* therefore
	be properly deinit'ed when the program exits or you will
	introduce a memory leak that will lead to unstable operation!)
	The BmpContainer array that holds these is declared global so
	this module's page_start_tick() routine (and evenually possibly
	others) can access then while animating.
	
	At any point the user can hit any of the Right Hand Side
	buttons. The associated callback will set an abort_flag 
	and the timer routine will stop scheduling new timer ticks.
	Once this happens the timer tick routine will no longer be 
	called and control passed to the main program menu module.
	
	When either all animation is complete or the abort_flag
	is set a  TempusFugit local helper function calls to this
	module's page_start_cleanup() function. It also calls 
	a public support function in the menu_program.c module
	to reset that module's clock tick counter (which was being 
	incremented by the regular handle_tick() function) to
	provide a clean function start.
	
	The TempusFugit.c module's finish_splash_page() function 
	then posts a request to open the program main menu via
	a call to menu_program_ahow_page() function in that module
	and then removes the associated window for this module from 
	the stack so that the back button provides a seemless exit 
	to the PebbleOS parent window.

Things you can do:
				- Try adding additional icons
				- Try changing the timer periods
				  for different effects
				- Try adding another flag to allow
				  animating multiple imagesets
				- Most importantly - Have Fun!


   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/19/13
   ----------------------------------------------------------------


\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
//#include "resource_ids.auto.h"	// provides hook to program resources

#include "TempusFugit.h"			// provides global program defines
#include "page_start.h"				// provides local program defines	
#include "menu_program.h"			// provides local program defines	
#include "feature_calc.h"			// provides local program defines	

	
// --------------------------------------------------------
//			Declare Local Module variables
// --------------------------------------------------------

#define START_DELAY		1000			// initial pause before animation
#define IMAGE_DELAY		 250			// pause between image
#define TEXT_DELAY		 800			// pause between text blocks


//static TempusFugitLogoData logo_data;	// structure for tracking logo info
										// (e.g. which is currently visible, etc)

static Window start_window;				// Start page window

// ----------------------------------
// used to animate start page
// ----------------------------------
//#define TF_TIMER_INTERVAL		400		// default splash page ticks to 500 milliseconds
	
static AppTimerHandle page_timer;		// system app timer struct

static int  tick_count = 0;				// splash page counter tracks next icon to show
static bool skip_start = false;			// flag to indicate user elected to skip splash page

//static bool splash_done = false;		// flag to indicate splash page processing is done
//static void  finish_splash_page();	// helper function to clean up when animation done

// ----------------------------------
//       Window Layers
// ----------------------------------
static TextLayer tempus_layer;
static TextLayer fugit_layer;
static TextLayer version_layer;

//static void handle_appear();
//static void handle_disappear();
//static void handle_load();
//static void handle_unload();

static void clicked_down();
static void clicked_up();
static void clicked_select();
static void click_config_provider();
static void page_start_complete();


// ----------------------------------------------------------------------------
//               Public Window functions
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//			  page_start_show_window()
//
//			Show first app window
//  (called from TempusFugit handle_init routine)
// --------------------------------------------------------
void page_start_show_window() {

	window_stack_push(&start_window, true);

}  //page_start_show_page()

// --------------------------------------------------------
//			page_start_timer_start()
//
//		Set up for first animation event
//  (called from TempusFugit handle_init routine)
// --------------------------------------------------------
void page_start_timer_start() {

	page_timer = app_timer_send_event(tf_app_context, START_DELAY, 42);
	
}  // page_start_timer_start()

// --------------------------------------------------------
//			handle_page_start_timer()
//		(called from TempusFugit tick handler)
// --------------------------------------------------------
void handle_page_start_timer() {

// ----------------------------------------
//   clean up timer and check if done
// ----------------------------------------
	app_timer_cancel_event(tf_app_context, page_timer);	// no more ticks, please

	if(skip_start) {		//  flag set by click handlers in page_start module
		page_start_complete();
		return;
	}

	if (tick_count == (NUMBER_OF_LARGE_IMAGES+4)){	// we're done
		page_start_complete();	// clean up & show program menu window
		return;
	}	

// else we be animating...
	if(tick_count < (NUMBER_OF_LARGE_IMAGES - 1)) {
		layer_remove_from_parent(&page_start_logo_data.image_container[tick_count].layer.layer);
		page_start_logo_data.current_logo = page_start_logo_data.current_logo+1;
		layer_set_frame(&page_start_logo_data.image_container[page_start_logo_data.current_logo].layer.layer, GRect(24,2,100,115));
		layer_add_child(&start_window.layer, &page_start_logo_data.image_container[page_start_logo_data.current_logo].layer.layer);
		page_timer = app_timer_send_event(tf_app_context, IMAGE_DELAY, 42);
	}

	else if (tick_count == (NUMBER_OF_LARGE_IMAGES-1)){		// Delay before showing text...
		light_enable_interaction();
		page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	}
	else if (tick_count == NUMBER_OF_LARGE_IMAGES){		// Show "Tempus" field
		layer_add_child(&start_window.layer, &tempus_layer.layer);	
		page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	}
	else if (tick_count == (NUMBER_OF_LARGE_IMAGES+1)){	// Show "Fugit" field
		layer_add_child(&start_window.layer, &fugit_layer.layer);	
		page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	}	
	else if (tick_count == (NUMBER_OF_LARGE_IMAGES+2)){	// Show version field
		layer_add_child(&start_window.layer, &version_layer.layer);	
		page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	}
	else if (tick_count == (NUMBER_OF_LARGE_IMAGES+3)){	// Show version field
		page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	}

	tick_count++;
//	page_timer = app_timer_send_event(tf_app_context, TEXT_DELAY, 42);
	


}  // handle_page_start_timer()

// ----------------------------------------------------------------------------
//               private functions
// ----------------------------------------------------------------------------

// -------------------------------------------------------------
//				page_start_complete()
//
//       private helper function for timer tick routine
//       performs clean up when splash page done. 
// -------------------------------------------------------------
static void page_start_complete() {
	
	feature_calc_reset();		// clean up calculator's tick counter
								// (was being incremented by opening ticks)
	menu_program_show_page();	//    and show program menu window

//     remove splash page window from stack
// (so final back button goes directly to main menu)
	window_stack_remove(&start_window, false);

	skip_start = true;

}  // page_start_complete()


// --------------------------------------------------------------------------
//          click provider section
// --------------------------------------------------------------------------

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {

	skip_start = true;			//

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {

	skip_start = true;			//

}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {

	skip_start = true;			//

}  // clicked_down()

// --------------------------------------------------------
//			clicked_config_provider()
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// --------------------------------------------------------
//			page_start_init()
//
//         Build window layers here
// --------------------------------------------------------
void page_start_init(){

//	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

	// ---------------------------------------
	// Set up feature window handlers
	// ---------------------------------------
	window_init(&start_window, "Tempus Fugit Start");
 //   window_set_window_handlers(&start_window, (WindowHandlers) {
 //       .appear = (WindowHandler)handle_appear,
 //       .disappear = (WindowHandler)handle_disappear,
//		.load = handle_load,
//		.unload = handle_unload,
//    });

	window_set_fullscreen(&start_window, true);	// remove top bar and replace with debug layer 
	window_set_background_color(&start_window, GColorBlack);

	window_set_click_config_provider(&start_window, (ClickConfigProvider) click_config_provider);
	

// Init child_a layer at (0,0) and size (40, 46):
//	layer_init(&page_start_logo_data.image_container[0].layer.layer, GRect(0, 0, 40, 46));

	page_start_logo_data.current_logo = 0;		// initialize logo tracking
	layer_set_frame(&page_start_logo_data.image_container[0].layer.layer, GRect(24,2,100,115));
	layer_add_child(&start_window.layer, &page_start_logo_data.image_container[0].layer.layer);

// set up Text layers
//TextLayer tempus_layer;
//TextLayer fugit_layer;

	text_layer_init(&tempus_layer, GRect (21, 108, 70, 28));
	text_layer_set_text(&tempus_layer, "Tempus");
	text_layer_set_background_color(&tempus_layer, GColorBlack);
	text_layer_set_text_color(&tempus_layer, GColorWhite);
	text_layer_set_font(&tempus_layer, norm24);

	text_layer_init(&fugit_layer, GRect (86, 108, 75, 28));
	text_layer_set_text(&fugit_layer, "Fugit");
	text_layer_set_background_color(&fugit_layer, GColorBlack);
	text_layer_set_text_color(&fugit_layer, GColorWhite);
	text_layer_set_font(&fugit_layer, norm24);

	text_layer_init(&version_layer, GRect (28, 137, 90, 20));
	text_layer_set_text_alignment(&version_layer, GTextAlignmentCenter);
	text_layer_set_text(&version_layer, PROGRAM_VERSION);
	text_layer_set_background_color(&version_layer, GColorBlack);
	text_layer_set_text_color(&version_layer, GColorWhite);
	text_layer_set_font(&version_layer, norm18);


	
}  // page_start_init()


// --------------------------------------------------------
//			page_start_deinit()
// --------------------------------------------------------
void page_start_deinit() {

}  // page_start_deinit()
