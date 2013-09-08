/* -------------------------------------------------------------------------------------- *\

                      feature_timer.c

	Implementation file for Tempus Fugit countdown timer feature


	Called from: Main Program Menu via the public function
				  feature_timer_show_page()
   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/23/13
   ----------------------------------------------------------------

Theory of Operation:
	
	Before this module can be used, the TempusFugit.c handle_init()
	routine must call the public function feature_timer_init(). This
	creates the needed Window, populates it with the appropriate
	Layers and sets needed defaults. To present the Window to the
	user, the function feature_timer_show_page() is invoked from the
	Program Main Menu (found in menu_program.c) This public function
	schedules the Window to be pushed onto the stack and displayed.
	
	The module's public functions are called by the module's 
	menu callbacks (located in the separate file menu_calc.c and
	menu_calc_buttons.c). For a complete list of currently available
	function see the header file feature_stop.h. This
	header also includes defines for default program values 
	(e.g. default values for the modules ALERT and COUNT variables).
	Eventually, when PebbleOS allows persistent storage, these can
	migrate to a Preferences or Settings menu.



\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"			// pull in global declares
#include "feature_timer.h"			// pull in local declares
#include "menu_timer.h"				// pull in needed public declares
#include "menu_timer_buttons.h"		//
	

// --------------------------------------------------------
//     Declare local structures & function prototypes
//  (provdes forward references to prevent compiler errors) 
//      These are all declared static to avoid naming
//               collisions at link time.
// --------------------------------------------------------
static void handle_appear(Window *window);
static void handle_disappear(Window *window);
static void handle_load(Window *window);
static void handle_unload(Window *window);

static void click_config_provider();
static void select_single_click_handler();
static void select_long_click_handler();
static void up_single_click_handler();
static void down_single_click_handler();

// helper function to refresh displayed count & alert
static void feature_timer_update_fields();

// --------------------------------------------------------
//	  Define & allocate local Module variables
//  (these could probably all be declared static)
// --------------------------------------------------------

static TempusFugitAlarmData feature_timer_data;	// module data structure

static bool timer_running = false;				// toggled by stopwatch menu

static Window tf_timer_window;					// timer main window


// ----------------------------------------------------------------------------
//               public window functions
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//	         feature_timer_show_page()
//
//     exposes the Timer window
//      (called from Program menu)
// --------------------------------------------------------
void feature_timer_show_page() {

	// activate app window
	window_stack_push(&tf_timer_window, true);

}  // feature_timer_show_page()

// --------------------------------------------------------
//        feature_timer_toggle()
//
//   implements the Timer START/STOP feature
//       (called from Timer menu)
// --------------------------------------------------------
void feature_timer_toggle() {

	timer_running = !timer_running;

// TODO: remove debug code before release
//	if(timer_running) {
//		snprintf(feature_timer_data.msgs_buffer, 16, "toggle: - true");
//	}
//	else {
//		snprintf(feature_timer_data.msgs_buffer, 16, "toggle: - false");
//	}
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);

}  // feature_timer_toggle()

// --------------------------------------------------------
//        feature_timer_reset()
//
//   implements the Timer RESET feature
//       (called from Timer menu)
// --------------------------------------------------------
void feature_timer_reset() {

// stop timer
	timer_running = false;
	
// set timer to default value (should be configurable
// by user but for that we need persistent storage)
//	feature_timer_data.tick_count = TIMER_DEFAULT_COUNT;
//	feature_timer_update_fields();

	// **************************************************** 
	// TODO: Replace with calls to a global state manager
	//       and use Dict structure to hold data! This
	//       facilitate use of Smartphone as storage.
	// ****************************************************
	// Initialize function variable defaults
	feature_timer_data.button_action = COUNT;			// default button behavior

	feature_timer_data.tick_count   = TIMER_DEFAULT_COUNT;	// init default tick counter
	feature_timer_data.count_second = 0;					// & associated display
	feature_timer_data.count_minute = 0;					// counters
	feature_timer_data.count_hour   = 0;					//

	feature_timer_data.alert_count  = TIMER_DEFAULT_ALERT;	// init elapsed time alert count
	feature_timer_data.alert_second = 0;					// & clear
	feature_timer_data.alert_minute = 0;					// associated counters
	feature_timer_data.alert_hour   = 0;					//

	feature_timer_update_fields();

// TODO: remove temporary debug message
//	snprintf(feature_timer_data.msgs_buffer, 16, "reset: = true");
//	text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);

}  // feature_timer_reset()


// --------------------------------------------------------
//			feature_timer_buttons_get()
//
//	  Called from menu buttons action bar handler,
//    Returns current button action flag.
// --------------------------------------------------------
int feature_timer_buttons_get(){
	
	return feature_timer_data.button_action;	// one of COUNT | ALERT

}  // feature_timer_buttons_get()


// --------------------------------------------------------
//			feature_timer_buttons_set()
//
//		(called from menu buttons action bar handler)
//		      Set current button action flag.
// --------------------------------------------------------
void feature_timer_buttons_set(int buttons){
	
	feature_timer_data.button_action = buttons;
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "buttons now: %d", feature_stop_data.button_action);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}
	
}  // feature_timer_buttons_set()

// --------------------------------------------------------------------------
//      module private functions
//
// --------------------------------------------------------------------------
// --------------------------------------------------------
//			feature_timer_update_fields()
//
//    updates tick count for the calculator feature
//   and if window is visible, update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_timer_update_fields() {

// Update displayed displayed Elapsed Time layer
	feature_timer_data.count_second = feature_timer_data.tick_count%60;
	feature_timer_data.count_minute = (feature_timer_data.tick_count/60)%60;//%60;
	feature_timer_data.count_hour   = feature_timer_data.tick_count/3600;

// Update displayed displayed alert Time layer
	feature_timer_data.alert_second = feature_timer_data.alert_count%60;
	feature_timer_data.alert_minute = (feature_timer_data.alert_count/60)%60;//%60;
	feature_timer_data.alert_hour   = feature_timer_data.alert_count/3600;

// and reset the layers - COUNT
	snprintf(feature_timer_data.count_buffer, 10, "%02d:%02d:%02d",  feature_timer_data.count_hour, feature_timer_data.count_minute, feature_timer_data.count_second);
	text_layer_set_text(&feature_timer_data.countbuf_layer, feature_timer_data.count_buffer);

// reset ALERTS
	if(!feature_timer_data.alert_count) {
		snprintf(feature_timer_data.alert_buffer, 10, "< off >   ");
	}
	else if(feature_timer_data.alert_hour > 0) {
	snprintf(feature_timer_data.alert_buffer, 10, "%02dh %02dmin",  feature_timer_data.alert_hour, feature_timer_data.alert_minute);
	}
	else {
		snprintf(feature_timer_data.alert_buffer, 10, "%02d min", feature_timer_data.alert_minute);
	}
	text_layer_set_text(&feature_timer_data.alertbuf_layer, feature_timer_data.alert_buffer);

//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 16, "stop: - %d", feature_timer_data.tick_count);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}
	
}  // feature_timer_update_fields()


// --------------------------------------------------------
//        feature_timer_clear()
//
//   implements the Timer CLEAR feature
//       (called from Timer menu)
// --------------------------------------------------------
void feature_timer_clear() {

	feature_timer_data.tick_count = 0;
	feature_timer_update_fields();

// TODO: remove temporary debug message
//	snprintf(feature_timer_data.msgs_buffer, 16, "clear: = true");
//	text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);

}  // feature_timer_clear()


// ----------------------------------------------------------------------------
//               Private Module functions
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//        feature_timer_start()
//
//          initiates timer
//       (called from timer menu)
// --------------------------------------------------------
void feature_timer_start(){

	if(!timer_running) {
		timer_running = true;
	}

	feature_timer_data.tick_count = feature_timer_data.alert_count;

}  // feature_timer_start()

// --------------------------------------------------------
//        feature_timer_stop()
//
//   suspends timer - called when count reaches max.
// --------------------------------------------------------
void feature_timer_stop(){

	if(timer_running) {
		timer_running = false;
	}
	feature_timer_data.tick_count = 0;
	feature_timer_data.count_second = 0; 
	feature_timer_data.count_minute = 0;
	feature_timer_data.count_hour = 0;

	feature_timer_update_fields();

}  // feature_timer_stop()


// --------------------------------------------------------
//			feature_timer_tick()
//
//    updates tick count for the Timer feature
//         and update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_timer_tick() {

	if(!timer_running) return;

// Increment time counters
// Note - Stopwatch vibes once, Timer vibes twice
//        so you can have both running at once
	if((--feature_timer_data.tick_count) < 1) {
		feature_timer_stop();			// count has reached zero
		vibes_double_pulse();			// stop timer and trigger alert
	}  

// TODO: remove this temporary debug message
//	snprintf(feature_timer_data.msgs_buffer, 18, "tick: - %d", feature_timer_data.tick_count);
//	text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
	
	// now update timer count fields
	feature_timer_update_fields();
// --------------------------------------------------
//    Animate Logo by removing the old bitmap layer,
//       then putting it back with a new image
// --------------------------------------------------
	if(tf_animating) {
		layer_remove_from_parent(&timer_logo_data.image_container[timer_logo_data.current_logo].layer.layer);

// increment logo tracker
		timer_logo_data.current_logo = (timer_logo_data.current_logo+1)%4;


// Now add new logo back to screen
		layer_set_frame(&timer_logo_data.image_container[timer_logo_data.current_logo].layer.layer, GRect(7,46,40,46));
		layer_add_child(&tf_timer_window.layer, &timer_logo_data.image_container[timer_logo_data.current_logo].layer.layer);

	}


// Now check if Alerts set & if so, process
// Note that topwatch vibes once, Timer vibes twice
	if(feature_timer_data.alert_count > 0) {
		if(!(feature_timer_data.tick_count % feature_timer_data.alert_count)) {
//		vibes_long_pulse();
			vibes_double_pulse();		
		}
	}
// ---------------------------------------------------------
//   ** Note use of debugging flag and header here **
// If debugging flag set, update debug message in header
// ---------------------------------------------------------
//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 18, "window: %d - %d", tf_active_window, feature_timer_data.tick_count%60);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}

}  // feature_timer_tick()


// ----------------------------------------------------------------------------
//      Button Handler functions for this window
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//		Calculator Window Button Handlers
// --------------------------------------------------------
void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

//	feature_timer_toggle();	// start/stop timer here

// call timer menu here
	menu_timer_show_page();
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 18, "win: - %d - SELECT", tf_active_window);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}

}  //select_single_click_handler()

// --------------------------------------------------------
//		select_long_click_handler()
// --------------------------------------------------------
void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

// call timer menu here
	menu_timer_show_page();
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 23, "win: - %d - SELECT-LONG", tf_active_window);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}

}  //select_long_click_handler()

// --------------------------------------------------------
//		up_single_click_handler()
// --------------------------------------------------------
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if(feature_timer_data.button_action == COUNT) {
		feature_timer_data.tick_count = feature_timer_data.tick_count + 60;
		if((feature_timer_data.alert_count/3600) > 23) { // wrap around at 24 hours
			feature_timer_data.alert_count = 0;
		}	
	}
	else {
		feature_timer_data.alert_count = feature_timer_data.alert_count + 60;
		if((feature_timer_data.alert_count/3600) > 23) { // wrap around at 24 hours
			feature_timer_data.alert_count = 0;
		}
	}

// now update displayed count fields
	feature_timer_update_fields();// If debugging flag set, update debug message in header

// and if we're debugging, let us know
//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 18, "win: - %d - UP", tf_active_window);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}

}  //up_single_click_handler()


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if(feature_timer_data.button_action == COUNT) {
		if(feature_timer_data.tick_count < 60) {
			feature_timer_data.tick_count = 0;
		}
		else {
			feature_timer_data.tick_count = feature_timer_data.tick_count - 60;
		}	
	}

	else {
		if(feature_timer_data.alert_count  < 60) {  // can't go to negative time
			feature_timer_data.alert_count = 0;
		}

		else {
			feature_timer_data.alert_count = feature_timer_data.alert_count - 60;
		}
	}


// now update displayed fields
	feature_timer_update_fields();

	// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_timer_data.msgs_buffer, 18, "win: - %d - DOWN", tf_active_window);
//		text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}


}  //down_single_click_handler()

// --------------------------------------------------------
//			click_config_provider()
//	   (sets up button callbacks for this window)
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, Window *window) {

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

	config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
	
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
	
}  //click_config_provider()



// --------------------------------------------------------
//			feature_timer_deinit()
// --------------------------------------------------------
void feature_timer_deinit() {
	
}  // feature_timer_deinit()

// ----------------------------------------------------------------------------
//               Window Handlers
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//      handle_timer_load()
// --------------------------------------------------------

void handle_load(Window *window) {

}  // handle_load()

// --------------------------------------------------------
//      handle_timer_unload()
// --------------------------------------------------------
void handle_unload(Window *window) {

}  // handle_unload()

// --------------------------------------------------------
//      handle_appear()
// --------------------------------------------------------

void handle_appear(Window *window) {

}  // handle_appear

// --------------------------------------------------------
//      handle_disappear()
// --------------------------------------------------------
void handle_disappear(Window *window) {

}  // handle_disappear()

	
// -------------------------------------------------------
//  
// -------------------------------------------------------


// --------------------------------------------------------
//			feature_timer_init()
//
//         Build window layers here
// --------------------------------------------------------
void feature_timer_init(){

	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//	GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

	// ---------------------------------------
	// Set up feature window handlers
	// ---------------------------------------
	window_init(&tf_timer_window, "Timer");
    window_set_window_handlers(&tf_timer_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
    });

//	window_set_fullscreen(&tf_timer_window, true);	// remove top bar and replace with debug layer 
	window_set_background_color(&tf_timer_window, GColorWhite);
	
//  --------------------------------------
// Set up window click handlers
//  --------------------------------------
//foobar - add memorable string to allow quick finds
	window_set_click_config_provider(&tf_timer_window, (ClickConfigProvider) click_config_provider);

// init page logo animation
	timer_logo_data.current_logo = 0;		// initialize logo tracking
	layer_set_frame(&timer_logo_data.image_container[0].layer.layer, GRect(7,46,40,46));
	layer_add_child(&tf_timer_window.layer, &timer_logo_data.image_container[0].layer.layer);
	

	// Populate label strings
	strcpy(feature_timer_data.alert_label, "ALERT:");
//	strcpy(feature_timer_data.count_label, "COUNT:");


	// Populate buffer defaults
	strcpy(feature_timer_data.title_label, "- TIMER -");		// window title
	strcpy(feature_timer_data.alert_buffer, "1 min");			// range: 00:00:00 - 23:59:59
	strcpy(feature_timer_data.count_buffer, "00:00.00");		// range: 00:00:00 - 23:59:59

	strcpy(feature_timer_data.msgs_buffer, "Tempus Fugit");		// Set up default header
																// (can also be used used for
																// debug messages)

	feature_timer_reset();		// clear running flag, clear counter & buffers
	
//  -----------------------------------------------------
// set up "TIMER" title buffer
//  -----------------------------------------------------
	text_layer_init(&feature_timer_data.title_layer, GRect (2, 2, 140, 32));
	text_layer_set_text_alignment(&feature_timer_data.title_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_timer_data.title_layer, feature_timer_data.title_label);
	text_layer_set_background_color(&feature_timer_data.title_layer, GColorWhite);
	text_layer_set_text_color(&feature_timer_data.title_layer, GColorBlack);
	text_layer_set_font(&feature_timer_data.title_layer, norm28);
	layer_add_child(&tf_timer_window.layer, &feature_timer_data.title_layer.layer);	


//  -----------------------------------------------------
//      init content buffers layers
//  -----------------------------------------------------

// COUNT buffer
	snprintf(feature_timer_data.count_buffer, 10, "%02d:%02d:%02d",  feature_timer_data.count_hour, feature_timer_data.count_minute, feature_timer_data.count_second);
	text_layer_set_text(&feature_timer_data.countbuf_layer, feature_timer_data.count_buffer);

	text_layer_init(&feature_timer_data.countbuf_layer, GRect (50, 48, 82, 32));
	text_layer_set_text_alignment(&feature_timer_data.countbuf_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_timer_data.countbuf_layer, feature_timer_data.count_buffer);
	text_layer_set_background_color(&feature_timer_data.countbuf_layer, GColorWhite);
	text_layer_set_text_color(&feature_timer_data.countbuf_layer, GColorBlack);
	text_layer_set_font(&feature_timer_data.countbuf_layer, norm28);
	layer_add_child(&tf_timer_window.layer, &feature_timer_data.countbuf_layer.layer);

// Function Label - "ALERT:"
	text_layer_init(&feature_timer_data.alert_layer, GRect (4, 104, 52, 28));
	text_layer_set_text(&feature_timer_data.alert_layer, feature_timer_data.alert_label);
	text_layer_set_background_color(&feature_timer_data.alert_layer, GColorWhite);
	text_layer_set_text_color(&feature_timer_data.alert_layer, GColorBlack);
	text_layer_set_font(&feature_timer_data.alert_layer, norm24);
	layer_add_child(&tf_timer_window.layer, &feature_timer_data.alert_layer.layer);

// Set up ALERT count buffer
	text_layer_init(&feature_timer_data.alertbuf_layer, GRect (56, 104, 86, 28));
	text_layer_set_text_alignment(&feature_timer_data.alertbuf_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_timer_data.alertbuf_layer, feature_timer_data.alert_buffer);
	text_layer_set_background_color(&feature_timer_data.alertbuf_layer, GColorWhite);
	text_layer_set_text_color(&feature_timer_data.alertbuf_layer, GColorBlack);
	text_layer_set_font(&feature_timer_data.alertbuf_layer, norm24);
	layer_add_child(&tf_timer_window.layer, &feature_timer_data.alertbuf_layer.layer);

// set up header/debug msg buffer
/*
text_layer_init(&feature_timer_data.msgb_layer, GRect (0, 0, 144, 18));
	text_layer_set_text_alignment(&feature_timer_data.msgb_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);
	text_layer_set_background_color(&feature_timer_data.msgb_layer, GColorBlack);
	text_layer_set_text_color(&feature_timer_data.msgb_layer, GColorWhite);
	text_layer_set_font(&feature_timer_data.msgb_layer, norm14);
	layer_add_child(&tf_timer_window.layer, &feature_timer_data.msgb_layer.layer);	
*/
// now update displayed fields
	feature_timer_update_fields();

	
}  // feature_timer_init()


