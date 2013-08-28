/* -------------------------------------------------------------------------------------- *\

                      feature_stop.h

	Header file for Tempus Fugit stopwatch feature

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/19/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_STOPWATCH_H
#define TF_STOPWATCH_H

// ----------------------------------------------
//			Module defines
// ----------------------------------------------
#define STOP_DEFAULT_COUNT		0
#define STOP_DEFAULT_ALERT		5*60

// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------

extern void feature_stop_show_page();	// called from menu_program
										// to display Window

extern void feature_stop_toggle();		// These are all called
extern void feature_stop_clear();		// from menu_stop.c to
extern void feature_stop_reset();		// implement module functions

extern void feature_stop_init();		// called from TempusFugit.c
extern void feature_stop_tick();		// handle_init() routine
extern void feature_stop_deinit();		// to set up and close module

extern int feature_stop_buttons_get();	// called by menu_stop_buttons.
										// returns current button mode
// Following is called by menu_stop_buttons.c.
// sets current button mode
extern void feature_stop_buttons_set(int buttons); 

#endif