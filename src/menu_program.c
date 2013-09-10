/* -------------------------------------------------------------------------------------- *\

                      menu_program.c

	Implements Tempus Fugit Program Main Menu window
	
	Called:		Launch on program start

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Last Update:		8/17/13
   ----------------------------------------------------------------

	Menu Commands:
		Cost Calculator		- parameters PEOPLE + WAGES + TIME
		Stopwatch			- Stopwatch with optional alerts
		Countdown Timer		- Countdown timer with optional alerts

			-----

			Help			- Basic user instructions
			About			- Program copyright and support link
		

Possible Additional Future Program Menu Options:
			- START/STOP ANIMATION
			- START/STOP ALERTS
			- SHOW/HIDE CALCULATOR DATA (ATTENDEES & WAGES)
			- SHOW/HIDE DATA (ATTENDEES & WAGES)


   --------------------------------------------------------------------------------

   Implementation plan:
   
	1) (done) Create program menu

	2) (done) Implement each menu command
		- (done) callbacks required for START/STOP, RESET & CLEAR

	3) (done) Implement About
	
	4) (done) Implement Help
	
	
\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "menu_program.h"			// local module declares

#include "feature_calc.h"			// call in public prototypes
#include "feature_stop.h"			//
#include "feature_timer.h"			//
#include "feature_analog.h"			//
#include "feature_simplicity.h"		//

#include "page_about.h"				//
#include "page_help.h"				//
//#include "page_start.h"				//

// --------------------------------------------------------
//  local module defines and variable definitions
// --------------------------------------------------------
static void menu_program_appear();
static void menu_program_disappear();
static void menu_program_load();
static void menu_program_unload();

static void menu_program_calculator_callback();
static void menu_program_stopwatch_callback();
static void menu_program_timer_callback();
//static void menu_program_salaries_callback();
static void menu_program_help_callback();
static void menu_program_about_callback();

	
#define NUM_MENU_PROGRAM_SECTIONS		3
#define NUM_MENU_PROGRAM_FIRST_ITEMS	3
#define NUM_MENU_PROGRAM_SECOND_ITEMS	3
#define NUM_MENU_PROGRAM_THIRD_ITEMS	2

static struct TempusFugitProgramMenuData {
	SimpleMenuLayer menu_layer;
	SimpleMenuSection menu_sections[NUM_MENU_PROGRAM_SECTIONS];
	SimpleMenuItem first_menu_items[NUM_MENU_PROGRAM_FIRST_ITEMS];
	SimpleMenuItem second_menu_items[NUM_MENU_PROGRAM_SECOND_ITEMS];
	SimpleMenuItem third_menu_items[NUM_MENU_PROGRAM_SECOND_ITEMS];
} menu_program_data;


static Window menu_program_window;				// program primary menu window

// ----------------------------------------------------------------------------
//      public functions for this module
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//      handle_load()
// --------------------------------------------------------

// --------------------------------------------------------
//			menu_program_show_page()
//
// create program menu window
// --------------------------------------------------------
void menu_program_show_page() {

	window_stack_push(&menu_program_window, true);   // create menu window

}  // menu_program_show_page()

// ----------------------------------------------------------------------------
//      menu callback handlers for this window
// ----------------------------------------------------------------------------
void menu_program_calculator_callback(){

	feature_calc_show_page();

}

void menu_program_stopwatch_callback(){

	feature_stop_show_page();
}

void menu_program_timer_callback() {

	feature_timer_show_page();
}

//void menu_program_salaries_callback() {

//	page_salaries_show_page();
//	page_help_show_page(PROGRAM);	// placehold to keep compiler happy
	
//}

void menu_program_clock_left_callback() {

	feature_analog_config(LEFTHANDED, USESECONDS);
	feature_analog_show_page();
	
}

void menu_program_clock_right_callback() {

	feature_analog_config(RIGHTHANDED, USESECONDS);
	feature_analog_show_page();
	
}

void menu_program_clock_simplicity_callback() {

	feature_simplicity_show_window();
	
}

void menu_program_help_callback() {

	page_help_show_page(PROGRAM);
	
}

void menu_program_about_callback() {

	page_about_show_page();
}

// ----------------------------------------------------------------------------
//               window handlers
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//			menu_program_appear()
// --------------------------------------------------------
void menu_program_appear(){

//	tf_active_window = MENU_PROGRAM_WINDOW;

}  // menu_program_appear()
	
// --------------------------------------------------------
//			menu_program_appear()
// --------------------------------------------------------
void menu_program_disappear(){

//	tf_active_window = NO_WINDOW;		// assumes windows are removed before new ones added...

}  // menu_program_appear()
	

// --------------------------------------------------------
//			menu_program_load()
// --------------------------------------------------------
void menu_program_load(){

}  // menu_program_appear()
	
// --------------------------------------------------------
//			menu_program_unload()
// --------------------------------------------------------
void menu_program_unload(){

}  // menu_program_unload()

// --------------------------------------------------------
//			menu_program_init()
//
//     called by TempusFugit.c to initialize menu data
// --------------------------------------------------------
void menu_program_init() {

	// -------------------------------
// init window handlers
// -------------------------------
	window_init(&menu_program_window, "Tempus Fugit");
	
	window_set_background_color(&menu_program_window, GColorWhite);
	window_set_window_handlers(&menu_program_window, (WindowHandlers) {
        .appear = (WindowHandler)menu_program_appear,
        .disappear = (WindowHandler)menu_program_disappear,
 		.load = menu_program_load,
 		.unload = menu_program_unload,
	});

// -------------------------------
// Initialize first menu section
// -------------------------------
int menu_count = 0;
	menu_program_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Cost Calculator",
		.callback = menu_program_calculator_callback,
	};
	
	menu_program_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Stopwatch",
		.callback = menu_program_stopwatch_callback,
	};

	menu_program_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Timer",
		.callback = menu_program_timer_callback,
	};

	// -------------------------------
	// Initialize second menu section
	// -------------------------------
	menu_count = 0;
//	menu_program_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Salaries",
//		.callback = menu_program_help_callback,
//	};

		menu_program_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Clock (Left)",
		.callback = menu_program_clock_left_callback,
	};

		menu_program_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Clock (Right)",
		.callback = menu_program_clock_right_callback,
	};

		menu_program_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Simplicity",
		.callback = menu_program_clock_simplicity_callback,
	};
	// -------------------------------
	// Initialize third menu section
	// -------------------------------
	menu_count = 0;

			menu_program_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Help",
		.callback = menu_program_help_callback,
	};

	menu_program_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "About",
		.callback = menu_program_about_callback,
	};


	// -------------------------------
	//  Bind menu items to
	//  corresponding menu sections
	// -------------------------------
	menu_program_data.menu_sections[0] = (SimpleMenuSection) {
		.title = "(Timers...)",
		.num_items = NUM_MENU_PROGRAM_FIRST_ITEMS,
		.items = menu_program_data.first_menu_items,
	};

	menu_program_data.menu_sections[1] = (SimpleMenuSection) {
		.title = "(Watchfaces...)",
		.num_items = NUM_MENU_PROGRAM_SECOND_ITEMS,
		.items = menu_program_data.second_menu_items,
	};

	menu_program_data.menu_sections[2] = (SimpleMenuSection) {
		.title = "(Misc...)",
		.num_items = NUM_MENU_PROGRAM_THIRD_ITEMS,
		.items = menu_program_data.third_menu_items,
	};

	// -------------------------------
	//  initialize menu window
	// -------------------------------

	simple_menu_layer_init(&menu_program_data.menu_layer, menu_program_window.layer.frame, &menu_program_window, menu_program_data.menu_sections, NUM_MENU_PROGRAM_SECTIONS, NULL);
	layer_add_child(&menu_program_window.layer, &menu_program_data.menu_layer.menu.scroll_layer.layer);


}  // menu_program_init()

// --------------------------------------------------------
//			menu_program_deinit()
// --------------------------------------------------------
void menu_program_deinit() {


} // menu_program_deinit()
