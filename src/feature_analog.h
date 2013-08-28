/* -------------------------------------------------------------------------------------- *\

                      feature_analog.h

		Global Header file for Tempus Fugit Analog Clock feature

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/28/13
	


\* -------------------------------------------------------------------------------------- */
#ifndef TF_ANALOG_H
#define TF_ANALOG_H

// ----------------------------------------------
//			Module defines
// ----------------------------------------------
#define	LEFTHANDED		1		// sets clock direction
#define RIGHTHANDED		0		// sets clock direction
#define USESECONDS		true

// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------

extern void feature_analog_show_page();	// called from menu_program
										// to display Window
extern void feature_analog_init();		// called from TempusFugit.c
extern void feature_analog_tick();		// handle_init() routine
extern void feature_analog_config();	// to set up direction & hands


#endif