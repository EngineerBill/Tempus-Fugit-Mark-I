/* -------------------------------------------------------------------------------------- *\

                      menu_stop.h

	Header file for Tempus Fugit Stopwatch Function Menu

Use:	Include this header file in any module that needs to access
		The Action Bar Menu public functions.
		
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/24/13
	Last Update:		8/24/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_STOP_H
#define TF_MENU_STOP_H


// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------
extern void menu_stop_init();		// called from TempusFugit.c
extern void menu_stop_deinit();		// handle_init() routine
									// to set up and close module

extern void menu_stop_show_page();
extern void menu_stop_cleanup();

#endif