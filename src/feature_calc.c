/* -------------------------------------------------------------------------------------- *\

                      feature_calc.c

	Implementation file for Tempus Fugit meeting cost calculator feature

	Called from: Main Program Menu via the public function
				  feature_calc_show_page()

Theory of Operation:
	
	Before this module can be used, the TempusFugit.c handle_init()
	routine must call the public function feature_calc_init(). This
	creates the needed Window, populates it with the appropriate
	Layers and sets needed defaults. To present the Window to the
	user, the function feature_calc_show_page() is invoked from the
	Program Main Menu (found in menu_program.c) This public function
	schedules the Window to be pushed onto the stack and displayed.
	
	The module's' public functions are called by the module's 
	menu callbacks (located in menu_calc.c and menu_calc_buttons.c).
	For a complete list, see the header file feature_stop.h. This
	header also includes defines for default program values 
	(e.g. default values for the modules ALERT and COUNT variables).
	Eventually, when PebbleOS allows persistent storage, these can
	migrate to a Settings menu.


   ----------------------------------------------------------------
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	8/16/13
	Last Update:	8/19/13
   ----------------------------------------------------------------
   

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
//#include "resource_ids.auto.h"	// provides hook to program resources

#include "TempusFugit.h"			// pull in global declares
#include "feature_calc.h"			// pull in local declares
#include "menu_calc.h"				// pull ion needed public declares
#include "menu_calc_buttons.h"		//
	

// --------------------------------------------------------
//   Declare local structures & function prototypes
//  (forward references to prevent compiler errors) 
//  (these should all be declared static)
// --------------------------------------------------------	
static void click_config_provider();
static void select_single_click_handler();
static void select_long_click_handler();
static void up_single_click_handler();
static void  down_single_click_handler();

static void handle_appear(Window *window);
static void handle_disappear(Window *window);
static void handle_load(Window *window);
static void handle_unload(Window *window);


// Local Calculator Data structure
typedef struct {
// Window TextLayers
	TextLayer title_layer;		// Calculator Window title layer (not used?)

	TextLayer cost_layer;		// Calculator label layers
	TextLayer time_layer;		//
	TextLayer people_layer;		//
	TextLayer wages_layer;		//

	TextLayer costb_layer;		// Calculator value buffer layers
	TextLayer timeb_layer;		// (contains changing data)
	TextLayer peopleb_layer;	//
	TextLayer wagesb_layer;		//

//	TextLayer msgb_layer;		// used to display debug messages

// Window Attribute buffers
	char time_label[6];			// "TIME:"
	char people_label[11];		// "PEOPLE:"
	char wages_label[7];		// "WAGES:"

// Window Value buffers
	char cost_buffer[12];		// range: $0 - $9,999.99
	char time_buffer[10];		// range: 00:00 - 23:59:59
	char people_buffer[10];		// range: 1 - 999
	char wages_buffer[10];		// range: $8.00 - $999.99
	char msgs_buffer[24];		// debug message buffer
	
// calculator data
	int num_people;				// meeting participant counter
	int wages;					// salary index (*1000)

	int tick_count;				// tick event counter
	int second_count;			// elapsed time counters
	int minute_count;			//
	int hour_count;				//

	bool running;				// track whether timer running
	char *caller;				// name of calling routine
	int button_action;			// determines button activity (PEOPLE | WAGES | TIME )

} TempusFugitCalcData;


TempusFugitCalcData feature_calc_data;	// module data structure

static bool timer_running = false;	// toggled by main_menu

Window tf_calc_window;				// Calculator main window

static void feature_calc_update_fields();


// ----------------------------------------------------------------------------
//               Public Window functions
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//	         show_calc_window()
//
//     exposes the calculator window
//       (called from Program menu)
// --------------------------------------------------------
void feature_calc_show_page() {

	// activate app window
	window_stack_push(&tf_calc_window, true);

}  // feature_calc_show_page()

// --------------------------------------------------------
//        feature_calc_timer_toggle()
//
//   implements the calculator START/STOP feature
//       (called from Calculator menu)
// --------------------------------------------------------
void feature_calc_toggle(){

	timer_running = !timer_running;

}  // feature_calc_timer_toggle()

// --------------------------------------------------------
//         feature_calc_reset()
//
//      implements the calculator CLEAR feature
//  (resets the calculator's internal data structure)
//       (called from Calculator menu)
// --------------------------------------------------------
void feature_calc_reset(){

	timer_running = false;

	// ********************************************* 
	// TODO: Replace with calls to tf_state_manager
	//       and use Dict structure to hold data
	//       (so so we can share with smart phone!)
	// *********************************************
	// Initialize function variable defaults
	feature_calc_data.tick_count = CALC_DEFAULT_COUNT;	// clear tick counter
	feature_calc_data.num_people = CALC_DEFAULT_PEOPLE;	// program data
	feature_calc_data.wages = CALC_DEFAULT_WAGES;		// salary index (*1000)

	feature_calc_data.button_action = BUTTON_CALC_DEFAULT;	// init buttons behavior
//	feature_calc_data.button_action = PEOPLE;	// init buttons behavior
//	feature_calc_data.button_action = TIME;		// indicates how buttons behave
//	feature_calc_data.button_action = WAGES;	// indicates how buttons behave


	feature_calc_update_fields();


}  // feature_calc_reset()

// --------------------------------------------------------
//         feature_calc_clear()
//
//      implements the calculator CLEAR feature
//  (resets the calculator's internal data structure)
//       (called from Calculator menu)
// --------------------------------------------------------
void feature_calc_clear(){

// clear the counters
	feature_calc_data.tick_count   = 0;
	feature_calc_data.second_count = 0; 
	feature_calc_data.minute_count = 0;
	feature_calc_data.hour_count   = 0;

// and reset the layers
	feature_calc_update_fields();

//	snprintf(feature_calc_data.cost_buffer, 10, "$%d.%02d",  0, 0);
//	text_layer_set_text(&feature_calc_data.costb_layer, feature_calc_data.cost_buffer);
	
}  // feature_calc_clear()


// --------------------------------------------------------
//			feature_calc_buttons_get()
//
//		(called from menu buttons action bar handler)
// --------------------------------------------------------
int feature_calc_buttons_get(){
	
	return feature_calc_data.button_action;	// one of PEOPLE | WAGES | TIME

}  // feature_calc_buttons_get()


// --------------------------------------------------------
//			feature_calc_buttons_set()
//
//		(called from menu buttons action bar handler)
// --------------------------------------------------------
void feature_calc_buttons_set(int buttons){
	
	feature_calc_data.button_action = buttons;
	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 18, "buttons now: %d", feature_calc_data.button_action);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
//	}

	
}  // feature_calc_buttons_set()

// --------------------------------------------------------------------------
//      module private functions
// --------------------------------------------------------------------------
// --------------------------------------------------------
//			feature_calc_update_fields()
//
//    updates tick count for the calculator feature
//   and if window is visible, update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_calc_update_fields() {
	
// Update displayed Elapsed Time layer
	feature_calc_data.second_count = feature_calc_data.tick_count%60;
	feature_calc_data.minute_count = (feature_calc_data.tick_count/60)%60;
	feature_calc_data.hour_count   =  feature_calc_data.tick_count/3600;

	snprintf(feature_calc_data.time_buffer, 10, "%02d:%02d:%02d",  feature_calc_data.hour_count, feature_calc_data.minute_count, feature_calc_data.second_count);
	text_layer_set_text(&feature_calc_data.timeb_layer, feature_calc_data.time_buffer);

// -------------------------------------------------------------------------------
//        Update Cost field
//
// Okay, this section turned out to be fairly painful
// to get right. Here's an explanation of why we're doing
// it the way we're doing it.
//// formula is:
//    cost(per second) = salary / (52 weeks * 40 hours * minutes * seconds)
//    total cost = #attendees * cost(per second) *  count(in seconds)
//
// The formula for total cost per second is to divide the stated
// salary by (52 weeks * 40 hours * 60 minutes * 60 seconds)
// This gives a per second cost. Multiple this by the number
// of attendees and the elapsed time in seconds and you
// should get the total cost for an elapsed period.
//
// Let's start with the allowed ranges for each parameter:
// People - 1-99
// Wages  - $1,000 - $499,000 per year
// Time   - 0 seconds - 23 hours, 59 minutes, 59 seconds
// 
// First observation - an int on the Pebble
// is a 32 bit value. If you naively multiple the three
// parameters together when they are at their maximums
// you would get 4,268,246,400, which exceeds what can be
// stored in a signed int.
//
// Second observation - if you multiple the three parameters
// together when they are all at or near their minimums
// and then divide by the required divisor you will get
// values less than one. Since we're dealing with integer
// arithmatice, this is truncated to zero. This for short
// periods, small salaries and few people, your counter wont
// "interesting" results for significant period.
//
// So what do we do? We trick the system by reducing a couple
// of the terms e.g. highest salary and total divisor are each
// a multiple of 1000, so we cancel those terms. We then multply
// the count term when the count is small to allow us to display
// fractional cent or cent values for the early ticks.As the 
// calculated values get larger, we eliminate this scaling to
// avoid overflows and adjust the presentation to show the
// user how the numbers are changing.
//
// Once we've managed to do all that math while keeping the result
// in a usable form, we map it into a display buffer and update
// the corresponding text layer. PebbleOS will then take care of
// automatically updating the display for us.
//
// Phew - that was a pain, given the lack of an emulator
// and relative lack of debugging tools.
// So here's a trick I used - I wrote intermediate values
// to one of the Window label layers (feature_calc_data.time_layer,
// *.people_layer and *.wages_layer, as well as the Window header
// layer feature_calc_data.msgb_layer) so they would display. This
// allowed me to confirm problems and get things working. 
//
// So that's the details of how we can calculate costs that can
// range from a fraction of a cent to $570,004 and display
// them successfully in a Text layer capable of holding only 7 digits.
// Painful, but it works...
//
// -------------------------------------------------------------------------------

// To start, convert cost to a per minute rate

unsigned int people = feature_calc_data.num_people;
unsigned int wages  = feature_calc_data.wages;
unsigned int time   = feature_calc_data.tick_count;

unsigned int cost = people * wages * time;
	
	if (cost < 75) { // we have fractions of a cent
		cost = (cost*10000)/7488;
snprintf(feature_calc_data.cost_buffer, 10, "0.%02d cent",  cost);	
	}
	else if (cost < 7488) { // we have a single penny (so don't print "cents")
		cost = (cost*100)/7488;
//snprintf(feature_calc_data.cost_buffer, 10, "$$d.%02d", cost/100, ((cost%100)/100));	
		if(cost == 1) {
			snprintf(feature_calc_data.cost_buffer, 10, "%d cent", cost);	
		 }
		else if ( cost < 10) { // we have 2-9 pennies
			snprintf(feature_calc_data.cost_buffer, 10, "%2d cents", cost);	
		}
		else { // we have 10-99 pennies
			snprintf(feature_calc_data.cost_buffer, 10, "%02d cents", cost);	
		}
	}
	else if (cost < 7488000) { // we have less than $1k, so show cents
		cost = cost*100/7488;
		snprintf(feature_calc_data.cost_buffer, 10, "$%d.%02d",  cost/100, ((cost)%100));
	}
	else {
		cost = cost/7488; // we're greater than $1k so don't show cents
		snprintf(feature_calc_data.cost_buffer, 10, "$%d",  cost);
	}

// formatting is done, so update the COST text layer
	text_layer_set_text(&feature_calc_data.costb_layer, feature_calc_data.cost_buffer);

// TODO: debugging code - remove at some point
//	snprintf(feature_calc_data.people_label, 7, "c=%d", cost);
//	text_layer_set_text(&feature_calc_data.people_layer, feature_calc_data.people_label);
//	text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);

	snprintf(feature_calc_data.people_buffer, 7, "%d", feature_calc_data.num_people);
	text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);

// write to label buffer for debugging	
//	snprintf(feature_calc_data.wages_label, 9, "$%d,000", feature_calc_data.wages);
//	text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_label);

	snprintf(feature_calc_data.wages_buffer, 9, "$%d,000", feature_calc_data.wages);
	text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_buffer);

//	if(tf_debugging) {
//	snprintf(feature_calc_data.msgs_buffer, 16, "cost: - %d", cost);
//	text_layer_set_text(&feature_timer_data.msgb_layer, feature_timer_data.msgs_buffer);  // create debug layer...
//	}
	
}  // feature_timer_update_fields()

// --------------------------------------------------------
//			feature_calc_tick()
//
//    updates tick count for the calculator feature
//   and if window is visible, update the Text layers
//   (called from TempusFugit:handle_tick() routine)
// --------------------------------------------------------
void feature_calc_tick() {

	if(!timer_running) return;

// Else increment elapsed time counter
	feature_calc_data.tick_count++;

// --------------------------------------------------
//    Animate Logo by removing the old bitmap layer,
//       then putting it back with a new image
// --------------------------------------------------
	if(tf_animating) { // will eventually make this a user preference
		layer_remove_from_parent(&calc_logo_data.image_container[calc_logo_data.current_logo].layer.layer);

// increment logo tracker (cycles through each in turn)
		calc_logo_data.current_logo = (calc_logo_data.current_logo+1)%4;

// Now add a new logo back to screen
		layer_set_frame(&calc_logo_data.image_container[calc_logo_data.current_logo].layer.layer, GRect(4,17,40,46));
		layer_add_child(&tf_calc_window.layer, &calc_logo_data.image_container[calc_logo_data.current_logo].layer.layer);

	}

// now update displayed COST and TIME fields
	feature_calc_update_fields();

// TODO: Eventually remove debugging code
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 18, "window: %d - %d", tf_active_window, feature_calc_data.tick_count%60);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
//	}

}  // feature_calc_tick()

// ----------------------------------------------------------------------------
//      button handler callbacks for this window
// ----------------------------------------------------------------------------
static void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

//	feature_calc_toggle();			// start/stop timer here
	menu_calc_show_page();			// show calculator menu


// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 18, "win: - %d - SELECT", tf_active_window);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
//	}

}  //select_single_click_handler()

// --------------------------------------------------------
//		select_long_click_handler()
// --------------------------------------------------------
static void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

	menu_calc_show_page();

	// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 23, "win: - %d - SELECT-LONG", tf_active_window);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
//	}
	
}  //select_long_ingle_click_handler()

// --------------------------------------------------------
//		up_single_click_handler()
// --------------------------------------------------------
static void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

// Which parameter we adjust is determined by button_action flag
	switch(feature_calc_data.button_action) {
	case PEOPLE:
		if((++feature_calc_data.num_people) > CALC_MAX_PEOPLE) {
			feature_calc_data.num_people = 1;
		}
		snprintf(feature_calc_data.people_buffer, 8, "%d", feature_calc_data.num_people);
		text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);
		break;

	case TIME:
		feature_calc_data.tick_count = feature_calc_data.tick_count + 60;
		if((feature_calc_data.tick_count) > (CALC_MAX_TIME-1)) {
			feature_calc_data.tick_count = 0;
		}
		break;

	case WAGES:
		if((++feature_calc_data.wages) > CALC_MAX_WAGES) {
			feature_calc_data.wages = 1;
		}
		snprintf(feature_calc_data.wages_buffer, 9, "$%d,000", feature_calc_data.wages);
		text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_buffer);
		break;
	}

// now update displayed COST and TIME fields
	feature_calc_update_fields();

	// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 18, "win: - %d - UP", tf_active_window);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
//	}
}  //up_single_click_handler()


static void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {


	switch(feature_calc_data.button_action) {
	case PEOPLE:
		if(--feature_calc_data.num_people < 1){
			feature_calc_data.num_people = CALC_MAX_PEOPLE;
		}
		snprintf(feature_calc_data.people_buffer, 8, "%d", feature_calc_data.num_people);
		text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);
		break;

	case TIME:  //  decrement time by one minutes

		if(feature_calc_data.tick_count  < 60) {  // can't go to negative time
			feature_calc_data.tick_count = CALC_MAX_TIME;
		}
		else {
// else decrement by 1 minute(layer will be reset on tick)
		feature_calc_data.tick_count = feature_calc_data.tick_count - 60;

		}
		break;

	case WAGES:
		if(--feature_calc_data.wages < 1) {
			feature_calc_data.wages = CALC_MAX_WAGES;
		}
// now reset layer
		snprintf(feature_calc_data.wages_buffer, 9, "$%d,000", feature_calc_data.wages);
		text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_buffer);
		break;
	}

// now update displayed COST and TIME fields
	feature_calc_update_fields();

	
// If debugging flag set, update debug message in header
//	if(tf_debugging) {
//		snprintf(feature_calc_data.msgs_buffer, 18, "win: - %d - DOWN", tf_active_window);
//		text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);  // create debug layer...
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

// ----------------------------------------------------------------------------
//      window handler callbacks for this window
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//      handle_load()
// --------------------------------------------------------

static void handle_load(Window *window) {

}  // handle_load

// --------------------------------------------------------
//      handle_unload()
// --------------------------------------------------------
static void handle_unload(Window *window) {

}  // handle_unload()

// --------------------------------------------------------
//      handle_appear()
// --------------------------------------------------------

static void handle_appear(Window *window) {

}  // handle_appear

// --------------------------------------------------------
//      handle_disappear()
// --------------------------------------------------------
static void handle_disappear(Window *window) {

}  // handle_disappear()


// --------------------------------------------------------
//			feature_calculator_init()
//
//         Build window layers here
// --------------------------------------------------------
void feature_calc_init(){

	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	//GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

	// ---------------------------------------
	// Set up feature window handlers
	// ---------------------------------------
	window_init(&tf_calc_window, "Tempus Fugit");
    window_set_window_handlers(&tf_calc_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
    });

//	window_set_fullscreen(&tf_calc_window, true);		// remove top bar and replace with debug layer 
	window_set_background_color( &tf_calc_window, GColorWhite);
	
//  --------------------------------------
// Set up window click handlers
//  --------------------------------------
//foobar - add memorable string to allow quick finds
	window_set_click_config_provider(&tf_calc_window, (ClickConfigProvider) click_config_provider);

// init page logo animation
	calc_logo_data.current_logo = 0;		// initialize logo tracking
	layer_set_frame(&calc_logo_data.image_container[0].layer.layer, GRect(4,17,40,46));
	layer_add_child(&tf_calc_window.layer, &calc_logo_data.image_container[0].layer.layer);
	

	// Populate label strings
	strcpy(feature_calc_data.time_label, "TIME:");		// range: 00:00 - 23:59:59
	strcpy(feature_calc_data.people_label, "PEOPLE:");	// range: 1 - 999
	strcpy(feature_calc_data.wages_label, "WAGES:");	// set labels

	// Populate buffer defaults
//	strcpy(feature_calc_data.cost_buffer, "$0.00");				// range: $0 - $9,999.99
//	strcpy(feature_calc_data.time_buffer, "00:00:00");			// range: 00:00 - 23:59:59
//	strcpy(feature_calc_data.people_buffer, "1");				// range: 1 - 999
//	strcpy(feature_calc_data.wages_buffer, "$100,000");			// range: $0 - $999,999
//	strcpy(feature_calc_data.msgs_buffer, "Tempus Fugit");	// Set up default header
															// (can also be used used for
															// debug messages)
	
//  -----------------------------------------------------
// Set up Function labels ("TIME", "PEOPLE"" & "WAGES")
//  -----------------------------------------------------
// set up "TIME" label
	text_layer_init(&feature_calc_data.time_layer, GRect (5, 62, 55, 28));
	text_layer_set_text(&feature_calc_data.time_layer, feature_calc_data.time_label);
	text_layer_set_background_color(&feature_calc_data.time_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.time_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.time_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.time_layer.layer);	

// set up "PEOPLE" label
	text_layer_init(&feature_calc_data.people_layer, GRect (5, 91, 70, 28));
	text_layer_set_text(&feature_calc_data.people_layer, feature_calc_data.people_label);
	text_layer_set_background_color(&feature_calc_data.people_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.people_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.people_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.people_layer.layer);	

// set up "Wages" label
	text_layer_init(&feature_calc_data.wages_layer, GRect (5, 120, 70, 28));
	text_layer_set_text(&feature_calc_data.wages_layer, feature_calc_data.wages_label);
	text_layer_set_background_color(&feature_calc_data.wages_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.wages_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.wages_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.wages_layer.layer);	


//  -----------------------------------------------------
//      init content buffers layers
//  -----------------------------------------------------

// set up COST buffer
	text_layer_init(&feature_calc_data.costb_layer, GRect (50, 20, 92, 32));
	text_layer_set_text_alignment(&feature_calc_data.costb_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_calc_data.costb_layer, feature_calc_data.cost_buffer);
	text_layer_set_background_color(&feature_calc_data.costb_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.costb_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.costb_layer, norm28);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.costb_layer.layer);	

// TIME buffer
	text_layer_init(&feature_calc_data.timeb_layer, GRect (63, 62, 73, 28));
	text_layer_set_text_alignment(&feature_calc_data.timeb_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_calc_data.timeb_layer, feature_calc_data.time_buffer);
	text_layer_set_background_color(&feature_calc_data.timeb_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.timeb_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.timeb_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.timeb_layer.layer);

// PEOPLE buffer
	text_layer_init(&feature_calc_data.peopleb_layer, GRect (60, 91, 64, 28));
	text_layer_set_text_alignment(&feature_calc_data.peopleb_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);
	text_layer_set_background_color(&feature_calc_data.peopleb_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.peopleb_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.peopleb_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.peopleb_layer.layer);

// WAGES buffer
	text_layer_init(&feature_calc_data.wagesb_layer, GRect (60, 120, 76, 28));
	text_layer_set_text_alignment(&feature_calc_data.wagesb_layer, GTextAlignmentRight);
	text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_buffer);
	text_layer_set_background_color(&feature_calc_data.wagesb_layer, GColorWhite);
	text_layer_set_text_color(&feature_calc_data.wagesb_layer, GColorBlack);
	text_layer_set_font(&feature_calc_data.wagesb_layer, norm24);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.wagesb_layer.layer);	


// set up header/debug msg buffer
/*
text_layer_init(&feature_calc_data.msgb_layer, GRect (0, 0, 144, 18));
	text_layer_set_text_alignment(&feature_calc_data.msgb_layer, GTextAlignmentCenter);
	text_layer_set_text(&feature_calc_data.msgb_layer, feature_calc_data.msgs_buffer);
	text_layer_set_background_color(&feature_calc_data.msgb_layer, GColorBlack);
	text_layer_set_text_color(&feature_calc_data.msgb_layer, GColorWhite);
	text_layer_set_font(&feature_calc_data.msgb_layer, norm14);
	layer_add_child(&tf_calc_window.layer, &feature_calc_data.msgb_layer.layer);	
*/

//  -----------------------------------------------------
//      now init content buffers
// (Note: we cannot do this until the layers have
//        been created by calls to text_layer_init())
//  -----------------------------------------------------
// TODO: remove this (hard coded for debugging)
	feature_calc_data.num_people     = CALC_DEFAULT_PEOPLE;
	feature_calc_data.wages          = CALC_DEFAULT_WAGES;
	feature_calc_data.tick_count     = CALC_DEFAULT_TIME;
	
	snprintf(feature_calc_data.time_buffer, 3, "%d", feature_calc_data.tick_count);
	text_layer_set_text(&feature_calc_data.timeb_layer, feature_calc_data.time_buffer);

	snprintf(feature_calc_data.people_buffer, 3, "%d", feature_calc_data.num_people);
	text_layer_set_text(&feature_calc_data.peopleb_layer, feature_calc_data.people_buffer);

	snprintf(feature_calc_data.wages_buffer, 9, "$%d,000", feature_calc_data.wages);
	text_layer_set_text(&feature_calc_data.wagesb_layer, feature_calc_data.wages_buffer);


	feature_calc_reset();


	
}  // feature_calc_init()


// --------------------------------------------------------
//			feature_calc_deinit()
// --------------------------------------------------------
void feature_calc_deinit() {
	
}  // feature_calc_deinit()