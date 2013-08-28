/* -------------------------------------------------------------------------------------- *\

                      menu_timer_buttons.h

	Header file for Tempus Fugit Timer Action Bar Menu Module

Use:	Include this header file in any module that needs to access
		The Action Bar Menu public functions.
		
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/27/13
	Last Update:		8/27/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_TIMER_BUTTONS_H
#define TF_MENU_TIMER_BUTTONS_H

#define NUMBER_OF_CHOICES	2

// flags to determine BUTTON mode
#define BUTTON_CALC_COUNT	0
#define BUTTON_CALC_ALERT	1

//extern int BUTTON_TIMER_SELECTION[];

//extern int button_selects;

// call this function to display Timer Action Bar menu
extern void menu_timer_buttons_select();


#endif
