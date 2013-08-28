/* -------------------------------------------------------------------------------------- *\

                      page_help.h

	Header file for Tempus Fugit context-sensitive Help Module

Use:	Include this header file in any module that needs to access
		The Help Window's public functions.
	
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/18/13
	Last Update:		8/27/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_HELP_H
#define TF_PAGE_HELP_H

extern void page_help_init();		// call this to init module
extern void page_help_show_page();	// call this to display Help Window
									// See  page_help.c opening comments
									// block for calling details
									// or TempusFugit.c for defines for
									// suitable flags.

#endif