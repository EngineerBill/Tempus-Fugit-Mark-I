/* -------------------------------------------------------------------------------------- *\

                      feature_calc.h

	Header file for Tempus Fugit meeting calculator feature

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Last Update:		8/16/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_CALCULATOR_H
#define TF_CALCULATOR_H

#include "TempusFugit.h"

// ---------------------------------------------------
//		Public defines & structures for this module
// ---------------------------------------------------
#define CALC_DEFAULT_COUNT		0
#define CALC_DEFAULT_TIME		15*60			// seems about right for a briefing...
#define CALC_DEFAULT_PEOPLE		2				// minimum for a meeting, non?
#define CALC_DEFAULT_WAGES		100				// $120k is average Silicon Valley programmer salary(!)

#define CALC_MAX_TIME			((24*60*60)-1)	// WARNING! product of these three
#define CALC_MAX_PEOPLE			99				// can overflow signed int. This
#define CALC_MAX_WAGES			499				// puts upper bound on calculation

// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------
extern void feature_calc_show_page();

extern void feature_calc_init();
extern void feature_calc_tick();
extern void feature_calc_deinit();

extern void feature_calc_toggle();
extern void feature_calc_reset();
extern void feature_calc_clear();

extern int feature_calc_buttons_get();
extern void feature_calc_buttons_set(int buttons);


#endif