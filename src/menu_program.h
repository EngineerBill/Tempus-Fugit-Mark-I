/* -------------------------------------------------------------------------------------- *\

                      menu_program.h

	Header file for Tempus Fugit Program Menu Module

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Last Update:		8/16/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_PROGRAM_H
#define TF_MENU_PROGRAM_H

// ----------------------------------------------
//			Module declares
// ----------------------------------------------

// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------

// call this fuction to show program main menu
extern void menu_program_show_page();

extern void menu_program_init();		// call at program start
extern void menu_program_deinit();		// call at program close

#endif
