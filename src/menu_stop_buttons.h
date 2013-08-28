/* -------------------------------------------------------------------------------------- *\

                      menu_stop_buttons.h

	Header file for Tempus Fugit TStopwatch Action Bar Menu Module

  Use:	Include this header file in any module that needs to access
		The Action Bar Menu public functions.

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/26/13
	Last Update:		8/26/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_STOP_BUTTONS_H
#define TF_MENU_STOP_BUTTONS_H

#define NUMBER_OF_CHOICES	2

// flags to determine BUTTON mode
#define BUTTON_CALC_COUNT	0
#define BUTTON_CALC_ALERT	1

extern int BUTTON_STOP_SELECTION[];

// call this function to display Stopwatch Action Bar menu
extern void menu_stop_buttons_select();

#endif
