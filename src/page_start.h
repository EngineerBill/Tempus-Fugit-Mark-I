/* -------------------------------------------------------------------------------------- *\

                      page_start.h

	Header file for Tempus Fugit Start Window module


	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Last Update:		8/19/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_START_H
#define TF_PAGE_START_H

// call this function to display Start Window
extern void page_start_show_page();	// call to display this Window
extern void page_start_init();		// call to init module	
extern void page_start_deinit();	// to set up and close module
extern void page_start_tick();		// call to service animation tick
extern void page_start_cleanup();	// Call this to remove parent from
									// Window stack and return to function
									// Module



#endif