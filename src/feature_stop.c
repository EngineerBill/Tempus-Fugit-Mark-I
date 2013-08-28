/* -------------------------------------------------------------------------------------- *\

                      feature_stop.c

	Implementation file for Tempus Fugit stopwatch feature

	Called from: Main Program Menu via the public function
				  feature_stop_show_page(). 

Theory of Operation:
	
	Before this module can be used, the TempusFugit.c handle_init()
	routine must call the public function feature_stop_init(). This
	creates the needed Window, populates it with the appropriate
	Layers and sets needed defaults. To present the Window to the
	user, the function feature_stop_show_page() is invoked from the
	Program Main Menu (found in menu_program.c) This public function
	schedules the Window to be pushed onto the stack and displayed.
	
	The module's' public functions are called by the module's 
	menu callbacks (located in menu_stop.c and menu_stop_buttons.c).
	For a complete list, see the header file feature_stop.h. This
	header also includes defines for default program values
	(e.g. default values for the modules ALERT and COUNT variables).
	Eventually, when PebbleOS allows persistent storage, these can
	migrate to a Settings menu.


   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/23/13
   ----------------------------------------------------------------



\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
//#include "resource_ids.auto.h"		// provides hook to program resources

#include "TempusFugit.h"				// pull in global declares
#include "feature_stop.h"				// pull in local declares
#include "menu_stop.h"					// pull in needed public declares
#include "menu_stop_buttons.h"			//
	

// --------------------------------------------------------
//   Declare local structures & function prototypes
//  (forward references to prvent compiler errors) 
//  (these should all be declared static)
// --------------------------------------------------------	
static void handle_appear(Window *window);
static void handle_disappear(Window *window);
static void handle_load(Window *window);
static void handle_unload(Window *window);;

static void click_config_provider();
static void select_single_click_handler();
static void select_long_click_handler();
static void up_single_click_handler();
static void down_single_click_handler();

static void feature_stop_update_fields();

// --------------------------------------------------------
//	  Define & allocate local Module variables
//  (these could probably all be declared static)
// --------------------------------------------------------

TempusFugitAlarmData feature_stop_data;	// module data structure

static bool timer_running = false;		// toggled by stopwatch menu

Window tf_stop_window;					// stopwatch main window


// ----------------------------------------------------------------------------
//               public window functions
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//	         feature_stop_show_page()
//
//     exposes the stopwatch window
//      (called from Program menu)
// --------------------------------------------------------
void feature_stop_show_page() {

	// activate app window
	window_stack_push(&tf_stop_window, true);

}  // feature_stop_show_page()

// --------------------------------------------------------
//        feature_stop_toggle()
//
//   implements the stopwatch START/STOP feature
//       (called from stopwatch menu)
// --------------------------------------------------------
void feature_stop_toggle(){

	timer_running = !timer_running;

}  // feature_stop_toggle()
// --------------------------------------------------------
//        feature_stop_reset()
//
//   implements the stopwatch RESET feature
//       (called from stopwatch menu)
// --------------------------------------------------------
void feature_stop_reset(){

	timer_running = false;
	
	feature_stop_data.tick_count = STOP_DEFAULT_COUNT;
	feature_stop_data.alert_count = STOP_DEFAULT_ALERT;
	feature_stop_update_fields();

}  // feature_stop_reset()
// --------------------------------------------------------
//        feature_stop_clear()
//
//   implements the stopwatch CLEAR feature
//       (called from stopwatch menu)
// --------------------------------------------------------
void feature_stop_clear(){

	feature_stop_data.tick_count = 0;
	feature_stop_update_fields();

}  // feature_stop_clear()


// --------------------------------------------------------
//			feature_stop_buttons_get()
//
//		(called from menu buttons action bar handler)
// --------------------------------------------------------
int feature_stop_buttons_get(){
	
	return feature_stop_data.button_action;	// one of COUNT | ALERT

}  // feature_stop_buttons_get()


// --------------------------------------------------------
//			feature_stopc_buttons_set()
//
//		(called from menu buttons action bar handler)
//		      Set current button action flag.
// --------------------------------------------------------
void feature_stop_buttons_set(int buttons){
	
	feature_stop_data.button_action = buttons;
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "buttons now: %d", feature_stop_data.button_action);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}
	
}  // feature_stop_buttons_set()

// ----------------------------------------------------------------------------
//               private module functions
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//         feature_stop_timer_clear_count()
//
//      implements the stopwatch CLEAR feature
//  (resets the stopwatch's internal data structure)
//       (called from stopwatch menu)
// --------------------------------------------------------
void feature_stop_timer_clear_count(){

// clear the counters
	feature_stop_data.tick_count   = 0;
	feature_stop_data.count_second = 0; 
	feature_stop_data.count_minute = 0;
	feature_stop_data.count_hour   = 0;

// and reset the layers
	snprintf(feature_stop_data.count_buffer, 10, "%02d:%02d:%02d",  feature_stop_data.count_hour, feature_stop_data.count_minute, feature_stop_data.count_second);
	text_layer_set_text(&feature_stop_data.countbuf_layer, feature_stop_data.count_buffer);

//	snprintf(feature_stop_data.alert_buffer, 10, "%02d:%02d:%02d",  feature_stop_data.alert_hour, feature_stop_data.alert_minute, feature_stop_data.alert_second);
//	text_layer_set_text(&feature_stop_data.alertbuf_layer, feature_stop_data.alert_buffer);
	
}  // feature_stop_timer_clear_count()

// --------------------------------------------------------
//          feature_stop_tick_clear()
//
//       clears tick counter to allow 
//     skipping or ending the splash page
//  (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_stop_tick_clear(){

	feature_stop_data.tick_count = 0;
}  // feature_stop_tick_clear()


// --------------------------------------------------------
//			feature_stop_update_fields()
//
//    updates tick count for the calculator feature
//   and if window is visible, update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_stop_update_fields() {

// Update displayed displayed Elapsed Time layer
	feature_stop_data.count_second = feature_stop_data.tick_count%60;
	feature_stop_data.count_minute = (feature_stop_data.tick_count/60)%60;//%60;
	feature_stop_data.count_hour   = feature_stop_data.tick_count/3600;
	if(feature_stop_data.count_hour > 23) {	// then roll over day
		feature_stop_data.tick_count = 0;
		feature_stop_data.count_second = 0;
		feature_stop_data.count_minute = 0;
		feature_stop_data.count_hour   = 0;
	}
// Update displayed displayed Alert Time layer
	feature_stop_data.alert_second = feature_stop_data.alert_count%60;
	feature_stop_data.alert_minute = (feature_stop_data.alert_count/60)%60;//%60;
	feature_stop_data.alert_hour   = feature_stop_data.alert_count/3600;
	if(feature_stop_data.count_hour > 23) {	// then roll over day
		feature_stop_data.alert_count = 0;
		feature_stop_data.alert_second = 0;
		feature_stop_data.alert_minute = 0;
		feature_stop_data.alert_hour   = 0;
	}

//	snprintf(feature_stop_data.count_buffer, 10, "%02d:%02d:%02d",  feature_stop_data.count_hour, feature_stop_data.count_minute, feature_stop_data.count_second);
//	text_layer_set_text(&feature_stop_data.countbuf_layer, feature_stop_data.count_buffer);

// and reset the layers
	snprintf(feature_stop_data.count_buffer, 10, "%02d:%02d:%02d",  feature_stop_data.count_hour, feature_stop_data.count_minute, feature_stop_data.count_second);
	text_layer_set_text(&feature_stop_data.countbuf_layer, feature_stop_data.count_buffer);

// foobar ALERTS
	if(!feature_stop_data.alert_count) {
		snprintf(feature_stop_data.alert_buffer, 10, "< off >   ");
	}
	else if(feature_stop_data.alert_hour > 0) {
	snprintf(feature_stop_data.alert_buffer, 10, "%02dh %02dmin",  feature_stop_data.alert_hour, feature_stop_data.alert_minute);
	}
	else {
		snprintf(feature_stop_data.alert_buffer, 10, "%d min", feature_stop_data.alert_minute);
	}
		text_layer_set_text(&feature_stop_data.alertbuf_layer, feature_stop_data.alert_buffer);

//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 16, "stop: - %d", feature_stop_data.tick_count);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}
	
}  // feature_stop_update_fields()


// --------------------------------------------------------
//			feature_stop_tick()
//
//    updates tick count for the stopwatch feature
//   and if window is visible, update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_stop_tick() {

	if(!timer_running) return;

// Increment time counters
	feature_stop_data.tick_count++;
// --------------------------------------------------
//    Animate Logo by removing the old bitmap layer,
//       then putting it back with a new image
// --------------------------------------------------
	if(tf_animating) {
		layer_remove_from_parent(&stop_logo_data.image_container[stop_logo_data.current_logo].layer.layer);

// decrement logo tracker
// (Note: we have sand running "uphill" in this window)
		if(++stop_logo_data.current_logo == NUMBER_OF_IMAGES) {
			stop_logo_data.current_logo = 0;
		}

// Now add new logo back to screen
		layer_set_frame(&stop_logo_data.image_container[stop_logo_data.current_logo].layer.layer, GRect(8,60,40,46));
		layer_add_child(&tf_stop_window.layer, &stop_logo_data.image_container[stop_logo_data.current_logo].layer.layer);

	}
// now update timer count fields
	feature_stop_update_fields();

// Now check if Alerts set & process
	if(feature_stop_data.alert_count > 0) {
		if(!(feature_stop_data.tick_count % feature_stop_data.alert_count)) {
			vibes_long_pulse();;		// Stopwatch vibes once, timer vibes twice
		}
	}
// ---------------------------------------------------------
//   ** Note use of debugging flag and header here **
// If debugging flag set, update debug message in header
// ---------------------------------------------------------
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "window: %d - %d", tf_active_window, feature_stop_data.tick_count%60);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}

}  // feature_stop_tick()


// ----------------------------------------------------------------------------
//      Button Handler functions for this window
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//		Calculator Window Button Handlers
// --------------------------------------------------------
void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	menu_stop_show_page();
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "win: - %d - SELECT", tf_active_window);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}

}  //select_single_click_handler()

// --------------------------------------------------------
//		select_long_click_handler()
// --------------------------------------------------------
void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

	menu_stop_show_page();

// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 23, "win: - %d - SELECT-LONG", tf_active_window);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}

}  //select_long_click_handler()

// --------------------------------------------------------
//		up_single_click_handler()
// --------------------------------------------------------
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if(feature_stop_data.button_action == COUNT) {
		feature_stop_data.tick_count = feature_stop_data.tick_count + 60;
		if((feature_stop_data.alert_count/3600) > 23) { // wrap around at 24 hours
			feature_stop_data.alert_count = 0;
		}	
	}
	else {
		feature_stop_data.alert_count = feature_stop_data.alert_count + 60;
		if((feature_stop_data.alert_count/3600) > 23) { // wrap around at 24 hours
			feature_stop_data.alert_count = 0;
		}
	}

// now update displayed COUNT and ALERT fields
	feature_stop_update_fields();

// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "win: - %d - UP", tf_active_window);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
//	}

}  //up_single_click_handler()


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if(feature_stop_data.button_action == COUNT) {
		if(feature_stop_data.tick_count < 60) {
			feature_stop_data.tick_count = 0;
		}
		else {
			feature_stop_data.tick_count = feature_stop_data.tick_count - 60;
		}	
	}

	else {
		if(feature_stop_data.alert_count  < 60) {  // can't go to negative time
			feature_stop_data.alert_count = 0;
		}

		else {
			feature_stop_data.alert_count = feature_stop_data.alert_count - 60;
		}
	}

// now update displayed COUNT and ALERT fields
	feature_stop_update_fields();


// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_stop_data.msgs_buffer, 18, "win: - %d - DOWN", tf_active_window);
//		text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);  // create debug layer...
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
//			feature_stop_deinit()
// --------------------------------------------------------
void feature_stop_deinit() {
	
}  // feature_stop_deinit()

// ----------------------------------------------------------------------------
//               Window Handlers
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//      handle_load()
// --------------------------------------------------------

void handle_load(Window *window) {

}  // handle_load()

// --------------------------------------------------------
//      handle_unload()
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
//			feature_stop_init()
//
//         Build window layers here
// --------------------------------------------------------
void feature_stop_init(){

	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

	// ---------------------------------------
	// Set up feature window handlers
	// ---------------------------------------
	window_init(&tf_stop_window, "Stopwatch");
    window_set_window_handlers(&tf_stop_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
    });

	window_set_fullscreen(&tf_stop_window, true);	// remove top bar and replace with debug layer 
	window_set_background_color(&tf_stop_window, GColorWhite);
	
//  --------------------------------------
// Set up window click handlers
//  --------------------------------------
//foobar - add memorable string to allow quick finds
	window_set_click_config_provider(&tf_stop_window, (ClickConfigProvider) click_config_provider);

// init page logo animation
	stop_logo_data.current_logo = 0;		// initialize logo tracking
	layer_set_frame(&stop_logo_data.image_container[0].layer.layer, GRect(8,60,40,46));
	layer_add_child(&tf_stop_window.layer, &stop_logo_data.image_container[0].layer.layer);
	

	// Populate label strings
	strcpy(feature_stop_data.alert_label, "ALERT:");
//	strcpy(feature_stop_data.count_label, "COUNT:");


	// Populate buffer defaults
	strcpy(feature_stop_data.title_label, "- STOPWATCH -");		// window title
	strcpy(feature_stop_data.alert_buffer, "5 min");			// range: 00:00:00 - 23:59:59
	strcpy(feature_stop_data.count_buffer, "00:00:00");			// range: 00:00:00 - 23:59:59

	strcpy(feature_stop_data.msgs_buffer, "Tempus Fugit");		// Set up default header
																// (also used used for debug
																// messages during development)

	// ********************************************* 
	// TODO: Replace with calls to tf_state_manager
	//       and use Dict structure to hold data!
	// *********************************************
	// Initialize function variable defaults
	feature_stop_data.button_action = ALERT;	// indicates how buttons behave (changes COUNT | ALERT)

	feature_stop_data.tick_count   = STOP_DEFAULT_COUNT;	// clear tick counter
	feature_stop_data.count_second = 0;			// & associated display
	feature_stop_data.count_minute = 0;			// counters
	feature_stop_data.count_hour   = 0;			//

	feature_stop_data.alert_count   = STOP_DEFAULT_ALERT;	// set elapsed time alert
	feature_stop_data.alert_second = 0;			// to 15 minutes & clear
	feature_stop_data.alert_minute = 0;			// associated counters
	feature_stop_data.alert_hour   = 0;			//

	//  -----------------------------------------------------
// set up "STOP WATCH" title buffer
//  -----------------------------------------------------
	text_layer_init(&feature_stop_data.title_layer, GRect (2, 20, 140, 32));
	text_layer_set_text_alignment(&feature_stop_data.title_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_stop_data.title_layer, feature_stop_data.title_label);
	text_layer_set_background_color(&feature_stop_data.title_layer, GColorWhite);
	text_layer_set_text_color(&feature_stop_data.title_layer, GColorBlack);
	text_layer_set_font(&feature_stop_data.title_layer, norm28);
	layer_add_child(&tf_stop_window.layer, &feature_stop_data.title_layer.layer);	


//  -----------------------------------------------------
//      init content buffers layers
//  -----------------------------------------------------

// COUNT buffer
	snprintf(feature_stop_data.count_buffer, 10, "%02d:%02d:%02d",  feature_stop_data.count_hour, feature_stop_data.count_minute, feature_stop_data.count_second);
	text_layer_set_text(&feature_stop_data.countbuf_layer, feature_stop_data.count_buffer);

	text_layer_init(&feature_stop_data.countbuf_layer, GRect (50, 62, 82, 32));
	text_layer_set_text_alignment(&feature_stop_data.countbuf_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_stop_data.countbuf_layer, feature_stop_data.count_buffer);
	text_layer_set_background_color(&feature_stop_data.countbuf_layer, GColorWhite);
	text_layer_set_text_color(&feature_stop_data.countbuf_layer, GColorBlack);
	text_layer_set_font(&feature_stop_data.countbuf_layer, norm28);
	layer_add_child(&tf_stop_window.layer, &feature_stop_data.countbuf_layer.layer);

// Function Label - "ALERT:"
	text_layer_init(&feature_stop_data.alert_layer, GRect (4, 120, 52, 28));
	text_layer_set_text(&feature_stop_data.alert_layer, feature_stop_data.alert_label);
	text_layer_set_background_color(&feature_stop_data.alert_layer, GColorWhite);
	text_layer_set_text_color(&feature_stop_data.alert_layer, GColorBlack);
	text_layer_set_font(&feature_stop_data.alert_layer, norm24);
	layer_add_child(&tf_stop_window.layer, &feature_stop_data.alert_layer.layer);

// Set up ALERT count buffer
	text_layer_init(&feature_stop_data.alertbuf_layer, GRect (56, 120, 86, 28));
	text_layer_set_text_alignment(&feature_stop_data.alertbuf_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_stop_data.alertbuf_layer, feature_stop_data.alert_buffer);
	text_layer_set_background_color(&feature_stop_data.alertbuf_layer, GColorWhite);
	text_layer_set_text_color(&feature_stop_data.alertbuf_layer, GColorBlack);
	text_layer_set_font(&feature_stop_data.alertbuf_layer, norm24);
	layer_add_child(&tf_stop_window.layer, &feature_stop_data.alertbuf_layer.layer);

// set up header/debug msg buffer
	text_layer_init(&feature_stop_data.msgb_layer, GRect (0, 0, 144, 18));
	text_layer_set_text_alignment(&feature_stop_data.msgb_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_stop_data.msgb_layer, feature_stop_data.msgs_buffer);
	text_layer_set_background_color(&feature_stop_data.msgb_layer, GColorBlack);
	text_layer_set_text_color(&feature_stop_data.msgb_layer, GColorWhite);
	text_layer_set_font(&feature_stop_data.msgb_layer, norm14);
	layer_add_child(&tf_stop_window.layer, &feature_stop_data.msgb_layer.layer);	

	
	
}  // feature_stop_init()


