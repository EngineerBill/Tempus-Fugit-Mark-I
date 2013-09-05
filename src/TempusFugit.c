/* -------------------------------------------------------------------------------------- *\

                  Tempus Fugit - Mark I
					   (version 1.0)

		    Calculating the True Cost of Business...

 Demos a non-watchface app, with timer tick for animation & calculation 

	Modes:
		- Calculator:	real time or static calculations
						based upon Number of attendees,
						estimated averge salaries and
						elapsed time

		- Stopwatch:	adjustable alerts (defaults to 15 minutes)

		- Clock (Left)	Left handed analog watchface

		- Clock (Right)	Right handed analog watchface

         -------------------------------------------
			  
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/5/13
	Last Update:		8/27/13
	
	Version:		Mark I (beta v0.1)
	
				        (c) 2013,
				    Steamchest Chronicles
				Licensed under the MIT License
				(see License file for details)

         -------------------------------------------

	Wishlist:
			- Means of saving state between instantiations
			   (Planned Android State Manager to come or
			    Pebble Central provides persistent storage)

         -------------------------------------------

Implementation Plan:

	1)(done) Implement Program Menu (TempusFugit.c, menu_program.c)

	2) (done) Implement Calculator (feature_calc.c)
	
	3) (done) Implement Calculator Menus (menu_calc.c & menu_calc_buttons.c)
	
	4) (done) Implement About/Help menus (page_about.c, page_help.c)
	
	5) (done) Implement Animated Start Page (page_start.c)
	
	6) (done) Implement Stopwatch window (feature_stop.c)

	7) (done) Implement Stopwatch Menu windows (menu_stop.c & menu_stop_buttons.c)

	7) (done) Implement Timer window (feature_timer.c)

	7) (done) Implement Stopwatch Menu window (menu_timer.c & menu_timer_buttons.c)

	8) Implement Analog watchface
	
	9) (in progress) document and release...

Note: 1) Each module has a corresponding <module>.h file, which contains global
      function prototypes and declares. This can be safely included in any other
	  module that needs them (for example, to be able to call a public function).

	2) As far as possible, we avoid global variables, but  those that were used
	  were all defined in "TempusFugit_globals.h". This file is included once in
	  "TempusFugit.c", allocating the required space. To pick up any such globals,
	  a module should include the file "TempusFugit.h" at the start of its implementation.

	3) Additional documentation and description of the implementation of this program
	will be posted to the blog "steamca.wordpress.com" as time and resources permit.

\* -------------------------------------------------------------------------------------- */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "resource_ids.auto.h"		// system-generated hook to program resources

// ----------------------------------------------
//			program header
// ----------------------------------------------
// Unique program ID (required by PebbleOS for synch/updates)
#define MY_UUID {0x04, 0xB4, 0xA1, 0xF0, 0xFC, 0x56, 0x11, 0xE2, 0xB7, 0x78, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66}

PBL_APP_INFO(MY_UUID,
             "TempusFugit - Mk I",
             "SteamChest Chronicles",
             1, 0,
             RESOURCE_ID_IMAGE_MENU_ICON,	// this icon used in Pebble menu
             APP_INFO_STANDARD_APP			// tells OS this is not a watchface
			 								// so e.g. we can get button events
);


// --------------------------------------------------------
// Bring in global defines
// --------------------------------------------------------
#include "TempusFugit.h"			// share global module vars & functional prototypes
#include "TempusFugit_globals.h"	// create global variables & functions

#include "feature_calc.h"			// feature module declares & functiontypes
#include "feature_stop.h"			//
#include "feature_timer.h"			//
#include "feature_analog.h"			//

#include "menu_program.h"			// menu module declares & function prototypes
#include "menu_calc.h"				//
#include "menu_stop.h"				//
#include "menu_timer.h"				//
//#include "menu_settings.h"		// RFU - we eventually want to provide
//#include "menu_defaults.h"		// settings & defaults when we have persistent storage

#include "page_start.h"				// support Pages declares & function prototypes 
#include "page_help.h"				// 
#include "page_about.h"				// 

// ----------------------------------
// used to animate start page
// ----------------------------------
#define TF_TIMER_INTERVAL		400		// default splash page ticks to 500 milliseconds
	
static AppTimerHandle tf_timer;			// system app timer struct

static int tf_tick_count = 0;			// splash page counter tracks next icon to show

static bool tf_splash_done = false;		// flag to indicate splash page done
static bool tf_skip_start = false;		// flag to indicate user elected to skip splash page

static void  finish_splash_page();		// helper function to clean up when animation done


// ----------------------------------------------------------------------------
//			tf_skip_start_page()
//
//  public function called by page_start to tell us to skip splash page.
// Note: Animation routines should be moved out of this module into
//       page_start module...
// ----------------------------------------------------------------------------
void tf_skip_start_page() {

	tf_skip_start = true;

}

// ----------------------------------------------------------------------------
//				handle_tick()
//
//  called by system every TICK interval, in turn
//  it calls each function module that requires service.
// ----------------------------------------------------------------------------

void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
	
	feature_calc_tick();					// despatch tick events
	feature_stop_tick();					// to individual functions
	feature_timer_tick();					//
	feature_analog_tick();					//

}

// ----------------------------------------------------------------------------
//				handle_timer()
//
// The following section of code handles despatching timer ticks to the
// program splash page handler until the maximum number of required ticks
// is reached or the user pushes any key, at which time the splash page 
// processing is aborted. This is signaled by setting the tf_skip_start
// flag in the Splash Page Window click handlers.
//
//  Note: This section should probably me moved off to the page_start.c
//        module and serviced by a call from handle_timer() to
//        that module. 
// ----------------------------------------------------------------------------
void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
	
	app_timer_cancel_event(tf_app_context, tf_timer);	// no more ticks, please

	if(tf_splash_done) {	// error handler in case we miss a timer tick
		return;
	}
	if((tf_skip_start) && !(tf_splash_done)) {	//  flag set by click handlers in page_start module
		finish_splash_page();
		return;
	}

// else we be animating...
	if(tf_tick_count < NUMBER_OF_IMAGES) {  // animate graphics
		page_start_tick(tf_tick_count++);
		tf_timer = app_timer_send_event(tf_app_context, 250, 42);
	}
	else if(tf_tick_count < (NUMBER_OF_IMAGES + 3)) {  // animate graphics
		page_start_tick(tf_tick_count++);
		tf_timer = app_timer_send_event(tf_app_context, 1200, 42);
	}
	else {
		finish_splash_page();	// clean up & display program menu window
	}

}  // handle_timer()

// ----------------------------------------------------------------------------
//				finish_splash_page()
//
//       private helper function for timer tick routine
//       performs clean up when splash page done. 
// ----------------------------------------------------------------------------
static void finish_splash_page() {
	
	feature_calc_reset();		// clean up calculator's tick counter
								// (was being incremented by opening ticks)
	menu_program_show_page();	// now show program menu window and
	page_start_cleanup();		// remove splash page window from stack

	tf_skip_start = true;		//
	tf_splash_done = true;		// flag to indicate splash page done

}  // finish_splash_page()

// --------------------------------------------------------
//			handle_init()
//
//   called on program start. Set up individual modules.
// --------------------------------------------------------

void handle_init(AppContextRef ctx) {
  //(void)ctx;
	
// -------------------------------------------------
//     set up state and global graphics
// -------------------------------------------------
	tf_app_context = ctx;

// TODO: when we have persistent storage we'll reload it here

// ----------------------------------------------
// set up arrays of images for animated logos
// ----------------------------------------------

//  Set up to access the animation image resources
	resource_init_current_app(&APP_RESOURCES);

// now init bitmap containers (remember to deinit on exit!)
// (Note: Logos used for animation, Icons used for Action Bar menus)
// logo_image_container[] used for small logos on feature pages,
// page_start_logo_data[] used for large logos on Splash Page shown on launch
	for(int i=0; i<NUMBER_OF_IMAGES; i++) {
        bmp_init_container(IMAGE_RESOURCE_IDS[i], &calc_logo_data.image_container[i]);
        bmp_init_container(IMAGE_RESOURCE_IDS[i], &stop_logo_data.image_container[i]);
        bmp_init_container(IMAGE_RESOURCE_IDS[i], &timer_logo_data.image_container[i]);
        bmp_init_container(IMAGE_RESOURCE_LARGE_IDS[i], &page_start_logo_data.image_container[i]);
	}

	for(int i=0; i<NUMBER_OF_ICONS; i++) {
        bmp_init_container(IMAGE_RESOURCE_ICONS[i], &tf_icon_container[i]);
	}

// ---------------------------------------------------
//     Initialize program modules
// Note: we create first window in menu_program_init()
// ---------------------------------------------------

//   init program feature modules
	feature_calc_init();				// init Calculator feature
	feature_stop_init();				// init Stopwatch feature
	feature_timer_init();				// init countdown Timer feature
	feature_analog_init();				// init Analog Clock feature

//   init various menu modules
	menu_program_init();			// init program Home Page menu
	menu_calc_init();				// init program Main config menu
	menu_stop_init();				// (reserved for future use)
	menu_timer_init();				// (reserved for future use)
//	menu_settings_init();			// (reserved for future use)
//	menu_defaults_init();			// r(eserved for future use)


//   init program support pages modules
	page_start_init();				// init start page
	page_about_init();				// init about page
	page_help_init();				// init page
//	page_timer_init();				//

	
// and now launch the program via the splash page
	page_start_show_page();

// and start the splash pager timer
	tf_timer = app_timer_send_event(tf_app_context, 1000, 42);

}  // handle_init()


// --------------------------------------------------------
//			handle_deinit()
// --------------------------------------------------------

void handle_deinit() {
// clean up logo containers
	for(int i = 0; i< NUMBER_OF_IMAGES; i++){
		bmp_deinit_container(&calc_logo_data.image_container[i]);
		bmp_deinit_container(&stop_logo_data.image_container[i]);
		bmp_deinit_container(&timer_logo_data.image_container[i]);
		bmp_deinit_container(&page_start_logo_data.image_container[i]);
	}
	
// clean up any allocated resources on exit
	menu_program_deinit();
	feature_calc_deinit();
	page_about_deinit();
	page_start_deinit();

	
}  // handle_program_deinit()



// ----------------------------------------------------------------------------
//			pbl_main()
// ----------------------------------------------------------------------------

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
	.timer_handler = &handle_timer,
	.tick_info = {
	.tick_handler = &handle_tick,
//	.tick_units = MINUTE_UNIT		// uncomment to use minute ticks 
	.tick_units = SECOND_UNIT		// uncomment to use second ticks
	}
  };
  app_event_loop(params, &handlers);
}
