/* -------------------------------------------------------------------------------------- *\

                      page_start.h

	Header file for Tempus Fugit Start Window module


	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/19/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_START_H
#define TF_PAGE_START_H

extern void page_start_show_window();	// display Start Window
extern void page_start_timer_start();	// starts timer sequence

extern void handle_page_start_timer();	// call to service animation tick

extern void page_start_init();			// call to init this module
extern void page_start_deinit();		// call to clean up on exit


#endif