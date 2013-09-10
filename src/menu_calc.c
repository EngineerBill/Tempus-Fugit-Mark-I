/* -------------------------------------------------------------------------------------- *\

                      menu_calc.c

	Implements Tempus Fugit Main Help Menu window

	Called:		long select button

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13   - Created for Tempus Fugit
	Modified:			9/10/13   - Changed menu order, relabled
   ----------------------------------------------------------------

	Main Menu Commands:
		Button Mode		- invokes Action Bar menu to select
						  one of: People | Wages | Time
						 
		START/STOP		- timer controls
		RESET			- stops timer, resets Alert and Counter
		CLEAR			- clears tick counter, does not stop count

		Help			- Context-sensitive Help page
		About			- Brief description of the module's function	
   
			(c) 2013 SteamChest Enterprises
			   email:	info@steamchest.com
			   blog:	steamchest.wordpress.com (to come -> blog.steamchest.com)
			   web:		www.steamchest.com (to come)
   

   ----------------------------------------------------------------
   
	To Come:
	SALARIES ->		Table showing cost per month/week/day/hour/minute/second
					for selected salary. UP/DOWN buttons change selection 
					and update Calculator SalarY setting.
	

   ----------------------------------------------------------------

	Potential additional features:

	Profiles >
				Allow user to choose specific meeting Profiles
				(possible loaded from Phone App, which would allow
				creating/updating selections)

	Default Profiles:
		    name			size, duration	  People		   Salary		Vibes (min)
		-------------------------------------------------------------------
		BrainStorm			small, short	     2			   90,000        15
		Briefing			small, medium	     2			   90,000        30
		Staff				small, long		     2			   90,000        60
		Division			medium, short	     5			   90,000        15
		Corporate			large, short	    20			   90,000        15

	
	Note: Android App will thus serve as muissing "backing store".

	App interface would  allow:
						- saving current Watch settings on change/exit
						- creation and editing of arbitrary profiles
						- Push changes from Pebble to Android on user change
						- Push changes from Android to Pebble on user edit/save


	
   --------------------------------------------------------------------------------
   

   TODOs:
   
	1) (done) Create function menu

	2) (done) Implement each menu command (see above)

	3) (done) Implement Buttons Action Bar menu

	4) (done) Implement Help
	
	5) (done) Implement About
	

5) Possible Additional Options
			- Reverse Background (& Hourglass?)
			- START/STOP ANIMATION
			- SHOW/HIDE DATA (ATTENDEES & WAGES)



\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "TempusFugit.h"
#include "menu_calc_buttons.h"		// local module declares

#include "feature_calc.h"			// pulls in function prototypes 
#include "page_help.h"				// for needed windows & menus
#include "page_about.h"				//

static void handle_appear();
static void handle_disappear();
static void handle_load();
static void handle_unload();

static void menu_calc_start_callback();
static void menu_calc_clear_callback();
static void menu_calc_buttons_callback();
static void menu_calc_help_callback();
//static void menu_calc_settings_callback();
//static void menu_calc_about_callback();

// --------------------------------------------------------
//  local module defines and variable definitions
// --------------------------------------------------------
//#define NUM_MENU_CALC_SECTIONS 3	// uncomment to add a third section
#define NUM_MENU_CALC_SECTIONS 1
#define NUM_MENU_CALC_FIRST_ITEMS 6
//#define NUM_MENU_CALC_SECOND_ITEMS 3
//#define NUM_MENU_CALC_THIRD_ITEMS 1

static struct TempusFugitMainMenuData {
	SimpleMenuLayer menu_layer;
	SimpleMenuSection menu_sections[NUM_MENU_CALC_SECTIONS];
	SimpleMenuItem first_menu_items[NUM_MENU_CALC_FIRST_ITEMS];
//	SimpleMenuItem second_menu_items[NUM_MENU_CALC_SECOND_ITEMS];
//	SimpleMenuItem third_menu_items[NUM_MENU_CALC_THIRD_ITEMS];
} menu_calc_data;

static Window menu_calc_window;	// program primary menu window

// ------------------------------------------------------------------
//          public functions for this module
//// ------------------------------------------------------------------
// --------------------------------------------------------
//			menu_calc_show_page()
//
//         Display About window
// --------------------------------------------------------
void menu_calc_show_page() {

// -------------------------------
// create program menu window
// -------------------------------

	window_stack_push(&menu_calc_window, true);   // create menu window

}  // menu_calc_show_page()

// --------------------------------------------------------
//			menu_calc_cleanup()
//
//   remove window so user returns to parent function
// --------------------------------------------------------
void menu_calc_cleanup() {

	window_stack_remove(&menu_calc_window, false);

}  // menu_calc_cleanup()

// ------------------------------------------------------------------
//
//        Window callbacks section
//
// ------------------------------------------------------------------
// --------------------------------------------------------
//			handle_appear()
// --------------------------------------------------------
void handle_appear(){

}  // handle_appear()
	
// --------------------------------------------------------
//			handle_disappear()
// --------------------------------------------------------
void handle_disappear(){

}  // handle_calc_appear()

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
	
// ------------------------------------------------------------------
//
//        Menu callbacks section
//
//  Contains menu selection callbacks
// ------------------------------------------------------------------
void menu_calc_start_callback(){

	feature_calc_toggle();
	window_stack_pop(true);				// and return to program
}

void menu_calc_reset_callback(){

	feature_calc_reset();
	window_stack_pop(true);   // clear all settings and return

}

void menu_calc_clear_callback(){

	feature_calc_clear();
	window_stack_pop(true);   // clear all settings and return

}

void menu_calc_buttons_callback() {

	menu_calc_buttons_select();	// change button actions
//	window_stack_pop(true);		// then return to calc function
	
}

void menu_calc_settings_callback() {

		window_stack_pop(true);   // call Settings menu
}

void menu_calc_about_callback() {

	page_help_show_page(CALCULATOR_ABOUT);
//	window_stack_pop(true);   // call scrollable Help window

}

void menu_calc_help_callback() {

	page_help_show_page(CALCULATOR_HELP);
//	window_stack_pop(true);   // call scrollable Help window

}

// ------------------------------------------------------------------
//          program initialization section
//
//    Called by TempusFugit.c on launch to set up
//          window and configure this module.
// ------------------------------------------------------------------

// --------------------------------------------------------
//			menu_calc_init()
//
//     called by TempusFugit.c to initialize menu data
// --------------------------------------------------------
void menu_calc_init() {

// -------------------------------
// init window handlers
// -------------------------------
	window_init(&menu_calc_window, "Calculator Menu");
	window_set_background_color(&menu_calc_window, GColorWhite);
	window_set_window_handlers(&menu_calc_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
 		.load = handle_load,
		.unload = handle_unload,
    });

	
// -------------------------------
// Initialize first menu section
// -------------------------------
int menu_count = 0;

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "START/STOP",
		.callback = menu_calc_start_callback,
	};

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "RESET",
		.callback = menu_calc_reset_callback,
	};

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "CLEAR",
		.callback = menu_calc_clear_callback,
	};

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Settings ->",
//		.callback = menu_calc_settings_callback,
		.title = "Button Mode ->",
		.callback = menu_calc_buttons_callback,
	};

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Settings ->",
//		.callback = menu_calc_settings_callback,
		.title = "Help ->",
		.callback = menu_calc_help_callback,
	};

	menu_calc_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Settings ->",
//		.callback = menu_calc_settings_callback,
		.title = "About ->",
		.callback = menu_calc_about_callback,
	};

	// -------------------------------
	// Initialize third menu section
	// -------------------------------
//	menu_count = 0;
//	menu_calc_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "Help ->",
//		.callback = menu_calc_help_callback,
//	};

//	menu_calc_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
//		.title = "About ->",
//		.callback = menu_calc_about_callback,
//	};


	// -------------------------------
	//  Bind menu items to
	//  corresponding menu sections
	// -------------------------------
	menu_calc_data.menu_sections[0] = (SimpleMenuSection) {
		.num_items = NUM_MENU_CALC_FIRST_ITEMS,
		.items = menu_calc_data.first_menu_items,
	};

//	menu_calc_data.menu_sections[1] = (SimpleMenuSection) {
//		.title = "(More...)",
//		.num_items = NUM_MENU_CALC_SECOND_ITEMS,
//		.items = menu_calc_data.second_menu_items,
//	};
	
//	menu_calc_data.menu_sections[2] = (SimpleMenuSection) {
//		.num_items = NUM_MENU_CALC_THIRD_ITEMS,
//		.items = menu_calc_data.third_menu_items,
//	};

	// -------------------------------
	//  initialize menu window
	// -------------------------------

	simple_menu_layer_init(&menu_calc_data.menu_layer, menu_calc_window.layer.frame, &menu_calc_window, menu_calc_data.menu_sections, NUM_MENU_CALC_SECTIONS, NULL);
	layer_add_child(&menu_calc_window.layer, &menu_calc_data.menu_layer.menu.scroll_layer.layer);


}  // menu_calc_init()

// --------------------------------------------------------
//			menu_calc_deinit()
// --------------------------------------------------------
void menu_calc_deinit() {


} // menu_calc_deinit()



