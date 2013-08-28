/* -------------------------------------------------------------------------------------- *\

                      feature_timer.h

	Header file for Tempus Fugit countdown timer feature

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/24/13
	Last Update:		8/24/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_TIMER_H
#define TF_TIMER_H

	
// ----------------------------------------------
//			Module defines
// ----------------------------------------------
#define TIMER_DEFAULT_COUNT		60
#define TIMER_DEFAULT_ALERT		 0



// ----------------------------------------------
//			Module declares
// ----------------------------------------------

// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------

extern void feature_timer_show_page();		// called from menu_program

extern void feature_timer_toggle();			// called from menu_timer
extern void feature_timer_clear();			// called from menu_timer
extern void feature_timer_reset();			// called from menu_timer

extern void feature_timer_init();
extern void feature_timer_tick();
extern void feature_timer_deinit();

extern int feature_timer_buttons_get();
extern void feature_timer_buttons_set(int buttons);

#endif