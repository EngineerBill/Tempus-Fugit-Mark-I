/* -------------------------------------------------------------------------------------- *\

                      menu_stop.c

	Implements Tempus Fugit Stopwatch Function Help Menu

	Called:		Stopwatch module long_select_click_handler()

   --------------------------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/24/13
	Last Update:		8/24/13
   --------------------------------------------------------------------------------


	Stopwatch Menu Commands:
		START/STOP		- timer controls
		RESET			- stops timer, resets Alert and Counter
		CLEAR			- clears tick counter, does not stop count

		Buttons			- invokes Action Bar menu to select
						  Timer button function (can change count
						  or Alert)
						  
		Help			- Context-sensitive Help page
		About			- Brief description of the module's function	
   -------------------------------------------------------------------------------- 
   TODO: Final test, document and release

   Implementation Plan
	1) (done) Create feature menu
	2) (done) Implement each menu command (see above)
		- requires callbacks for START/STOP, CLEAR and RESET
	3) (done) Implement Help
		- Requires modifying page_help.c to take argument and add extra text	
	4) (done) Populate Feature Help Text

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "TempusFugit.h"			// Pull in global declares
#include "menu_stop.h"				// pull in local module declares
#include "menu_stop_buttons.h"		//

#include "feature_stop.h"			// pull in needed declares 
#include "page_help.h"				// & function prototypes

// --------------------------------------------------------
//  declare private function protoypes
// --------------------------------------------------------
static void handle_appear();
static void handle_disappear();
static void handle_load();
static void handle_unload();


// --------------------------------------------------------
//  local module defines and variable definitions
// --------------------------------------------------------
#define NUM_MENU_STOP_SECTIONS 2
#define NUM_MENU_STOP_FIRST_ITEMS 3
#define NUM_MENU_STOP_SECOND_ITEMS 3
//#define NUM_MENU_STOP_THIRD_ITEMS 1

static struct TempusFugitStopMenuData {
	SimpleMenuLayer menu_layer;
	SimpleMenuSection menu_sections[NUM_MENU_STOP_SECTIONS];
	SimpleMenuItem first_menu_items[NUM_MENU_STOP_FIRST_ITEMS];
	SimpleMenuItem second_menu_items[NUM_MENU_STOP_SECOND_ITEMS];
//	SimpleMenuItem third_menu_items[NUM_MENU_STOP_THIRD_ITEMS];
} menu_stop_data;

static Window menu_stop_window;				// program primary menu window

// --------------------------------------------------------
//  local module defines and variable definitions
// --------------------------------------------------------
// --------------------------------------------------------
//			menu_stop_show_page()
//
//         Display Stopwatch Menu window
// --------------------------------------------------------
void menu_stop_show_page() {

	window_stack_push(&menu_stop_window, true);   // create menu window

}  // menu_stop_show_page()

	
// ------------------------------------------------------------------
//
//        Menu Callbacks section
//
//  Contains menu selection callbacks
// ------------------------------------------------------------------
void menu_stop_start_callback(){

	feature_stop_toggle();			// Calls the Stopwatch feature module to 
	window_stack_pop(true);			// toggle its internal run state, pop
									// window & return
}

void menu_stop_clear_callback(){

	feature_stop_clear();			// clear Counter setting
	window_stack_pop(true);   		//  & return

}

void menu_stop_reset_callback() {

	feature_stop_reset();			// Reset Default Alert value, Clear Stopwatch count
	window_stack_pop(true);			// & return
	
}


void menu_stop_buttons_callback() {

	menu_stop_buttons_select();		// change button actions
//	window_stack_pop(true);			// then return to calc function
	
}

void menu_stop_about_callback() {

	page_help_show_page(STOPWATCH_ABOUT);	// Call context-sensitive Help page

}

void menu_stop_help_callback() {

	page_help_show_page(STOPWATCH_HELP);	// Call context-sensitive Help page

}

// --------------------------------------------------------
//			menu_stop_cleanup()
//
//   remove menu window so Action bar returns to function
// --------------------------------------------------------
void menu_stop_cleanup() {

	window_stack_remove(&menu_stop_window, false);

}  // menu_stop_cleanup()


// ------------------------------------------------------------------
//
//        Window callbacks section
//
//  Contains menu selection callbacks
// ------------------------------------------------------------------
// --------------------------------------------------------
//			handle_appear()
// --------------------------------------------------------
void handle_appear(){

}  // handle_appear()
	
// --------------------------------------------------------
//			handleu_disappear()
// --------------------------------------------------------
void handle_disappear(){

}  // handle_appear()

// --------------------------------------------------------
//			handle_load()
// --------------------------------------------------------
void handle_load(){

}  // handle_appear()
	
// --------------------------------------------------------
//			handle_unload()
// --------------------------------------------------------
void handle_unload(){

}  // handle_unload()

// --------------------------------------------------------
//			menu_stop_init()
//
//     called by TempusFugit.c to initialize menu data
// --------------------------------------------------------
void menu_stop_init() {

// -------------------------------
// init window handlers
// -------------------------------
	window_init(&menu_stop_window, "Stopwatch Menu");
	window_set_background_color(&menu_stop_window, GColorWhite);
	window_set_window_handlers(&menu_stop_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
 		.load = handle_load,
		.unload = handle_unload,
    });

// -------------------------------
// Initialize first menu section
// -------------------------------
int menu_count = 0;
	menu_stop_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "START/STOP",
		.callback = menu_stop_start_callback,
	};
	menu_stop_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "RESET",
		.callback = menu_stop_reset_callback,
	};	
	menu_stop_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "CLEAR",
		.callback = menu_stop_clear_callback,
	};

	// -------------------------------
	// Initialize second menu section
	// -------------------------------
	menu_count = 0;

	menu_stop_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Button Mode ->",
		.callback = menu_stop_buttons_callback,
	};

	menu_stop_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Help ->",
		.callback = menu_stop_help_callback,
	};

	menu_stop_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "About ->",
		.callback = menu_stop_about_callback,
	};

	// -------------------------------
	// Initialize third menu section
	// -------------------------------
//	menu_count = 0;
//	menu_stop_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Help ->",
//		.callback = menu_stop_help_callback,
//	};

//	menu_stop_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "About ->",
//		.callback = menu_stop_about_callback,
//	};


	// -------------------------------
	//  Bind menu items to
	//  corresponding menu sections
	// -------------------------------
	menu_stop_data.menu_sections[0] = (SimpleMenuSection) {
		.num_items = NUM_MENU_STOP_FIRST_ITEMS,
		.items = menu_stop_data.first_menu_items,
	};
	menu_stop_data.menu_sections[1] = (SimpleMenuSection) {
		.title = "(More...)",
		.num_items = NUM_MENU_STOP_SECOND_ITEMS,
		.items = menu_stop_data.second_menu_items,
	};
//	menu_stop_data.menu_sections[2] = (SimpleMenuSection) {
//		.num_items = NUM_MENU_STOP_THIRD_ITEMS,
//		.items = menu_stop_data.third_menu_items,
//	};

// -------------------------------
//  initialize menu window
// -------------------------------
	simple_menu_layer_init(&menu_stop_data.menu_layer, menu_stop_window.layer.frame, &menu_stop_window, menu_stop_data.menu_sections, NUM_MENU_STOP_SECTIONS, NULL);
	layer_add_child(&menu_stop_window.layer, &menu_stop_data.menu_layer.menu.scroll_layer.layer);

}  // menu__stop_init()

// --------------------------------------------------------
//			menu_stop_deinit()
// --------------------------------------------------------
void menu_stop_deinit() {

} // menu_stop_deinit()
