/* -------------------------------------------------------------------------------------- *\

                      page_about.h

	Header file for Tempus Fugit About page

	Called from :
					- Main Program menu

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/18/13
	Last Update:		8/18/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_ABOUT_H
#define TF_PAGE_ABOUT_H

	
// called to display Program About Window
extern void page_about_show_page();

extern void page_about_init();		// call to init module
extern void page_about_deinit();	// call to clean up module

#endif