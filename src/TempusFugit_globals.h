/* -------------------------------------------------------------------------------------- *\

                      TempusFugit_globals.h
					  
		   Global header file for Tempus Fugit program
	
	
	Includes definitions and space allocation for global
	        variables needed across modules.
	
	Note: variable declares and function prototypes are handled
		  in each module through a corresponding "<modulename>.h"" file.

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Last Update:		8/16/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_GLOBALS_H
#define TF_GLOBALS_H


// --------------------------------------------------------
//
//    Define global program variables
//
// (these should  eventually move to a state manager
// and be accessible only through public functions)
// --------------------------------------------------------
// -----------------------------------
//    Array of animation logos
// -----------------------------------
// animation for feature pages
int IMAGE_RESOURCE_IDS[NUMBER_OF_IMAGES] = {
	RESOURCE_ID_HOURGLASS_ZERO, RESOURCE_ID_HOURGLASS_ONE,
	RESOURCE_ID_HOURGLASS_TWO, RESOURCE_ID_HOURGLASS_THREE
};


// animation for opening splash page
int IMAGE_RESOURCE_LARGE_IDS[NUMBER_OF_LARGE_IMAGES] = {
	RESOURCE_ID_HOURGLASS_LARGE_ZERO, RESOURCE_ID_HOURGLASS_LARGE_ONE,
	RESOURCE_ID_HOURGLASS_LARGE_TWO, RESOURCE_ID_HOURGLASS_LARGE_THREE,
	RESOURCE_ID_HOURGLASS_LARGE_FOUR, RESOURCE_ID_HOURGLASS_LARGE_FIVE,
	RESOURCE_ID_HOURGLASS_LARGE_SIX
};

// icons for Action Bar menus
int IMAGE_RESOURCE_ICONS[NUMBER_OF_ICONS] = {
	RESOURCE_ID_UP_ARROW, RESOURCE_ID_TICK, RESOURCE_ID_DOWN_ARROW
};

// --------------------------------------------------------
//        Create global animation structures
//  Note: currently one per window.
//  TODO: convert to a single global function
//        to eliminate redundency/reduce space
// --------------------------------------------------------
// TODO: can shrink code by making this one set
TempusFugitLogoData calc_logo_data;			// Logo Animation data structures
TempusFugitLogoData stop_logo_data;			//
TempusFugitLogoData timer_logo_data;		//
TempusFugitLargeLogoData page_start_logo_data;	//

// -----------------------------------
//    Global Logo Animation vars
// -----------------------------------
// logo bitmap image containers

BmpContainer tf_icon_container[NUMBER_OF_ICONS]; // used by Action Bar menus...

int tf_active_window = NO_WINDOW;				// Track active window

AppContextRef tf_app_context;					// stores global App context

bool tf_animating = true;						// controls logo animation throughout program
bool tf_debugging = false;						// enables printing messages in header

#endif
