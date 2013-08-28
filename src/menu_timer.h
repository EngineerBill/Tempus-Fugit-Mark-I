/* -------------------------------------------------------------------------------------- *\

                      menu_timer.h

	Header file for Tempus Fugit Timer Function Menu Module

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/24/13
	Last Update:		8/24/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_TIMER_H
#define TF_MENU_TIMER_H


// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------
extern void menu_timer_init();		// call to init module	
extern void menu_timer_deinit();	// call to clean up module

extern void menu_timer_show_page();	// call this to display Timer Menu
extern void menu_timer_cleanup();	// Call this to remove parent from
									// Window stack and return to function
									// Module
#endif
