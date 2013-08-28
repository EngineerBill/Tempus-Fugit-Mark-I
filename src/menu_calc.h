/* -------------------------------------------------------------------------------------- *\

                      menu_calc.h

	Header file for Tempus Fugit Calculator Menu Module

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Last Update:		8/17/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_CALC_H
#define TF_MENU_CALC_H


// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------
extern void menu_calc_init();		// call on program start
extern void menu_calc_deinit();		// call on program exit

extern void menu_calc_show_page();	// call to display this Window
extern void menu_calc_cleanup();	// call to remove menu
									// from stack, allowing
									// direct return to function

	
#endif