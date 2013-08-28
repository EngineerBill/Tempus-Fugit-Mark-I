/* ------------------------------------------------------------------ *\

                      TempusFugit.h

	Program declares for TempusFugit Pebble app.
 
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/8/13
	Last Update:		8/8/13

\* ------------------------------------------------------------------ */	
#ifndef TEMPUS_FUGIT_H
#define TEMPUS_FUGIT_H

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


// ----------------------------------------------
//		Public program declares
// ----------------------------------------------

// Feature defines used e.g. when calling 
// context sensitive help function
#define PROGRAM				0
#define CALCULATOR_ABOUT	1
#define CALCULATOR_HELP		2
#define STOPWATCH_ABOUT		3
#define STOPWATCH_HELP		4
#define TIMER_ABOUT			5
#define TIMER_HELP			6


// Calculator parameter defines
#define PEOPLE				0
#define TIME				1
#define WAGES				2

// Stopwatch & Timer parameter defines
#define COUNT				0
#define ALERT				1	

// -----------------------------------
//    Track active window
// -----------------------------------
// ***********************************
// TODO: Make this an enum list
//       and move to State Manager.
// ***********************************	
#define NUMBER_OF_WINDOWS		 9
#define NO_WINDOW				-1
#define MENU_PROGRAM_WINDOW		 0
#define CALC_WINDOW				 1
#define TIMER_UP_WINDOW			 2
#define TIMER_DOWN_WINDOW		 3
#define MENU_MAIN_WINDOW		 4
#define MENU_SETTINGS_WINDOW	 5
#define SETTINGS_WINDOW			 6
#define ABOUT_WINDOW			 7
#define HELP_WINDOW				 8

//#define DEFAULTS_WINDOW		 9	// to come
//#define SETTING_WINDOW		 10	// to come


#define NUMBER_OF_IMAGES		4
extern int IMAGE_RESOURCE_IDS[];
extern int IMAGE_RESOURCE_LARGE_IDS[];

#define NUMBER_OF_ICONS			3
extern int IMAGE_RESOURCE_ICONS[];

// -----------------------------------
//    Global Logo Animation vars
// -----------------------------------


// Provides logo animation data for each feature window
typedef struct {
	Layer animation_layer;							// logo slideshow layer
	BmpContainer image_container[NUMBER_OF_IMAGES];	// logo container
	int current_logo;								// tracks which is active aswe cycle
} TempusFugitLogoData;

typedef struct {
// Window TextLayers
	TextLayer title_layer;		// Alarm Window title layer

	TextLayer count_layer;		// displays timer count & 
	TextLayer alert_layer;		// alert labels

	TextLayer countbuf_layer;	// Stopwatch value buffers
	TextLayer alertbuf_layer;	// (contains changing data)

	TextLayer msgb_layer;		// used to display debug messages

// buffers to hold window labels
	char title_label[15];		// "STOPWATCH", "- TIMER -", etc
	char alert_label[7];		// "ALERT:"

// Window Value buffers
	char count_buffer[10];		// range: 00:00:00 - 23:59:59
	char alert_buffer[10];		// range: 00:00 - 23:59

	char msgs_buffer[24];		// debug message buffer
	
	int tick_count;				// tick event counter
	int count_second;			// elapsed time display
	int count_minute;			//
	int count_hour;				//

	int alert_count;			// holds vibe alert target
	int alert_second;			// alert display
	int alert_minute;			//
	int alert_hour;				//

	bool running;				// track whether timer running
	char *caller;				// name of calling routine
	int button_action;			// determines button activity (may be COUNT | ALERT)

} TempusFugitAlarmData;

// Logo Animation data structures
extern TempusFugitLogoData calc_logo_data;			// small logos (for feature pages)		
extern TempusFugitLogoData stop_logo_data;			// 		
extern TempusFugitLogoData timer_logo_data;			// 		
extern TempusFugitLogoData page_start_logo_data;	// large logo (for splash page)

extern BmpContainer tf_icon_container[];  			// holds Action Bar icons

// -----------------------------------
//    Global Program Variables
// -----------------------------------

extern int tf_active_window;			// Make public active window flag
extern AppContextRef tf_app_context;	// Store global app context

extern bool tf_animating;				// controls logo animation throughout program
extern bool tf_debugging;				// enables printing messages in header

// -----------------------------------
//      Public functions
// -----------------------------------

extern void tf_skip_start_page();		// hitting "Select" on splash page skips

#endif