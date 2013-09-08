/* ------------------------------------------------------------------ *\

               SteamCab Left Handed Analog - Mark II

               A counter-revolutionary approach to time...

Modifies the example analog watchface app for counterclockwise operation.
Also demonstrates adding a TextLayer to an existing watchface app.



	Author:				Pebble Central
	Modified:			EngineerBill (engineerbill@stemchest.com)
	Date Created:		8/5/13
	Last Modified:		8/28/13  - Integrated into TempusFugit
	
	Release History:
	
		Mark I		- counterclockwise watchface, TextLayer
		
		Mark II		- Removed Second Hand (customer request)

		Mark III	- Integrated into TempusFugit.
							- converted from Watchface App to standard App
							- Added flag to allow selection of watch direction
							- Added flag to select/ommit second hand
							
About:
------

Steamchest Enterprises' first foray into the world of Pebble Watchface design.

Our initial investigations into the world of Pebble programming focused on
understanding the structure and requirements for a working Watchface app.
After studying a number of examples and reviewing what was already out there,
we decided to roll our own. The result was the 'SteamCab Left Handed Analog - Mark I',
a "counter-revolutionary" approach to analog watch design. A follow-on Mark II release
eliminated the second hand at the request of redditor "/reddit.com/u/arsirion".

This project began life as the example "simple_analog" example watch in
thePebble SDK. The original was modified to provide counterclockwise hand
movement and a new layer containing a text string. We then made an annotation
pass through the source code to help those who would follow.

The changes to convert to counterclock operation are quite minor, but the
module serves to teach the basic structure of a watchface app and the steps
needed to add and display a TextBox to a window. 


You can find the original example source code in the Pebble SDK
watches directory:

       "SDK-1.12->Pebble->watches->simple_analog".


Contact Us:
-----------

email:  engineerbill@steamchest.com
web:    www.steamchest.com (coming soon)

blog:   steamcab.wordpress.com (documenting my voyage into Pebble programming)


The Steamchest Chronicles:
--------------------------

       (c) 2013
 the Steamchest Chronicles

\* ------------------------------------------------------------------ */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "feature_analog_globals.h"
#include "feature_analog.h"

/* -------------------------------------------------------- *\
              private declares Section
\* -------------------------------------------------------- */


// --------------------------------------------------------
//              SteamCab Declares
// --------------------------------------------------------

static int left_handed = 1;	// set to 0 to run right-handed, 1 to run left-handed
static int second_hand = 0;	// set to 0 to hide second hand, 1 to show

// --------------------------------------------------------
//              Original Simple Analog Declares
// --------------------------------------------------------
static struct SimpleAnalogData {
  Layer simple_bg_layer;

  Layer date_layer;

  TextLayer title_label;   // added for SteamCab title
  char title_buffer[12];   // added for SteamCab title
	
  TextLayer version_label; // added for SteamCab version number
  char version_buffer[6];  // added for SteamCab version number

  TextLayer day_label;
  char day_buffer[6];

  TextLayer num_label;
  char num_buffer[4];

  GPath minute_arrow, hour_arrow;
  GPath tick_paths[NUM_CLOCK_TICKS];
  Layer hands_layer;
  Window window;          // clock window
} s_data;

// -------------------------------------------------------------
//              public clock routines
// -------------------------------------------------------------
// --------------------------------------------------------
//           feature_analog_show_page()
//
//   callback handler supplied to system during init
// --------------------------------------------------------
void feature_analog_show_page() {

	window_stack_push(&s_data.window, true);

}  // feature_analog_show_page()

// --------------------------------------------------------
//           feature_analog_config()
//
//   callback handler supplied to system during init
// --------------------------------------------------------
void feature_analog_config(int direction, int hands) {
// set global module flags
	left_handed = direction;		// called from Program menu
	second_hand = hands;			//

}  // feature_analog_config() 

// --------------------------------------------------------
//             feature_analog_tick())
// --------------------------------------------------------
void feature_analog_tick(AppContextRef ctx, PebbleTickEvent* t) {

		layer_mark_dirty(&s_data.window.layer);

}  // handle_analog_tick()


// -------------------------------------------------------------
//              Clock Update Routines
// -------------------------------------------------------------
// --------------------------------------------------------
//           bg__update_proc()
//
//   callback handler supplied to system during init
// --------------------------------------------------------
void bg_update_proc(Layer* me, GContext* ctx) {

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, me->bounds, 0, GCornerNone);

  graphics_context_set_fill_color(ctx, GColorWhite);
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_draw_filled(ctx, &s_data.tick_paths[i]);
  }
}  // bg_update_proc()

// --------------------------------------------------------
//           hands_update_proc()
//
//   callback handler supplied to system during init
// --------------------------------------------------------
void hands_update_proc(Layer* me, GContext* ctx) {
  const GPoint center = grect_center_point(&me->bounds);
  const int16_t secondHandLength = me->bounds.size.w / 2;

  GPoint secondHand;
  PblTm t;
  int leftTime;
  get_time(&t);

// ------------------------------------------------
//   Remove this section to eliminate second hand  
// ------------------------------------------------

	if(second_hand) {
	// Added for Left Handed Operation
		if(left_handed) {
		  	leftTime = 60 - t.tm_sec;
		}
		else {
			leftTime = t.tm_sec;
		}
		
	// calculate new angle
	//  int32_t second_angle = TRIG_MAX_ANGLE * t.tm_sec / 60;  // uncomment for right-handed operation
		int32_t second_angle = TRIG_MAX_ANGLE * leftTime / 60;  // uncomment for left-handed operation

		secondHand.y = (int16_t)(-cos_lookup(second_angle) * (int32_t)secondHandLength / TRIG_MAX_RATIO) + center.y;
		secondHand.x = (int16_t)(sin_lookup(second_angle) * (int32_t)secondHandLength / TRIG_MAX_RATIO) + center.x;

  // draw second hand
		graphics_context_set_stroke_color(ctx, GColorWhite);
		graphics_draw_line(ctx, secondHand, center);
	}
	
  // minute/hour hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);

	// Added for Left Handed Operation
		if(left_handed) {
		  	leftTime = 60 - t.tm_min;
		}
		else {
			leftTime = t.tm_min;
		}

	//  gpath_rotate_to(&s_data.minute_arrow, TRIG_MAX_ANGLE * t.tm_min / 60);
  gpath_rotate_to(&s_data.minute_arrow, TRIG_MAX_ANGLE * leftTime / 60);
  gpath_draw_filled(ctx, &s_data.minute_arrow);
  gpath_draw_outline(ctx, &s_data.minute_arrow);

	// Added for Left Handed Operation
	//  TODO: bug in clockwise hour setting
		if(left_handed) {
		  	leftTime = 12 - t.tm_hour;
			gpath_rotate_to(&s_data.hour_arrow, (TRIG_MAX_ANGLE * (((leftTime % 12) * 6) - (t.tm_min / 10))) / (12 * 6));
 		}
		else {
			leftTime = t.tm_hour;
			gpath_rotate_to(&s_data.hour_arrow, (TRIG_MAX_ANGLE * (((leftTime % 12) * 6) + (t.tm_min / 10))) / (12 * 6));
 		}

//  gpath_rotate_to(&s_data.hour_arrow, (TRIG_MAX_ANGLE * (((t.tm_hour % 12) * 6) + (t.tm_min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, &s_data.hour_arrow);
  gpath_draw_outline(ctx, &s_data.hour_arrow);

  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(me->bounds.size.w / 2-1, me->bounds.size.h / 2-1, 3, 3), 0, GCornerNone);

}  // hands_update_proc()

// --------------------------------------------------------
//           date_update_proc()
//
//   callback handler supplied to system during init
// --------------------------------------------------------
void date_update_proc(Layer* me, GContext* ctx) {

  PblTm t;
  get_time(&t);

  string_format_time(s_data.day_buffer, sizeof(s_data.day_buffer), "%a", &t);
  text_layer_set_text(&s_data.day_label, s_data.day_buffer);

  string_format_time(s_data.num_buffer, sizeof(s_data.num_buffer), "%d", &t);
  text_layer_set_text(&s_data.num_label, s_data.num_buffer);

}  // date_update_proc()
// --------------------------------------------------------------------------
//          click provider section
// --------------------------------------------------------------------------

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_down()

// --------------------------------------------------------
//			clicked_config_provider()
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
     config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
   config[BUTTON_ID_DOWN]->click.handler = clicked_down;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// --------------------------------------------------------
//             feature_analog_init()
//			  
//		Initialize Analog Clock on start
// --------------------------------------------------------
void feature_analog_init() {

	left_handed = LEFTHANDED; 	// sets clock direction
	second_hand   = true;		// show minute hand

  // create clock window 
  window_init(&s_data.window, "SteamCab Analog");
 //  --------------------------------------
// Set up window click handlers
//  --------------------------------------
//foobar - add memorable string to allow quick finds
	window_set_click_config_provider(&s_data.window, (ClickConfigProvider) click_config_provider);

  // set up new Text Layer, hand paths, init layers, hands, etc
  // Change text here to customize for your needs... 
  strcpy(s_data.title_buffer, "SteamChest");	// Max buffer sizes are currently 9 chars & 5 chars!! 
  strcpy(s_data.version_buffer, ".com");		// (to allow for trailing nulls)
  s_data.day_buffer[0] = '\0';					// used to hold current date
  s_data.num_buffer[0] = '\0';					// 

  // init hand paths
  gpath_init(&s_data.minute_arrow, &MINUTE_HAND_POINTS);
  gpath_init(&s_data.hour_arrow, &HOUR_HAND_POINTS);

  const GPoint center = grect_center_point(&s_data.window.layer.bounds);
  gpath_move_to(&s_data.minute_arrow, center);
  gpath_move_to(&s_data.hour_arrow, center);

  // init clock face paths
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_init(&s_data.tick_paths[i], &ANALOG_BG_POINTS[i]);
  }

  // init layers
  layer_init(&s_data.simple_bg_layer, s_data.window.layer.frame);
  s_data.simple_bg_layer.update_proc = &bg_update_proc;
  layer_add_child(&s_data.window.layer, &s_data.simple_bg_layer);

  // init date layer -> a plain parent layer to create a date update proc
  layer_init(&s_data.date_layer, s_data.window.layer.frame);
  s_data.date_layer.update_proc = &date_update_proc;
  layer_add_child(&s_data.window.layer, &s_data.date_layer);

//  added for SteamCab - init title on face
  text_layer_init(&s_data.title_label, GRect(38, 27, 87, 18));			// tweak here to adjust title location
  text_layer_set_text(&s_data.title_label, s_data.title_buffer);		// this is 40 pixels across, 27 down
  text_layer_set_background_color(&s_data.title_label, GColorBlack);	// 87 wide, 18 high
  text_layer_set_text_color(&s_data.title_label, GColorWhite);
  GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  text_layer_set_font(&s_data.title_label, norm18);

  layer_add_child(&s_data.window.layer, &s_data.title_label.layer);

//  added for SteamCab - init extension on face (was to be version, now ".com")
  text_layer_init(&s_data.version_label, GRect(56, 45, 57, 18));			// tweak here to adjust version location
  text_layer_set_text(&s_data.version_label, s_data.version_buffer);		// this is 56 pixels across, 45 down 
  text_layer_set_background_color(&s_data.version_label, GColorBlack);		// 57 wide, 18 high
  text_layer_set_text_color(&s_data.version_label, GColorWhite);
  text_layer_set_font(&s_data.version_label, norm18);

  layer_add_child(&s_data.window.layer, &s_data.version_label.layer);

  // init day
  text_layer_init(&s_data.day_label, GRect(46, 114, 27, 20));
  text_layer_set_text(&s_data.day_label, s_data.day_buffer);
  text_layer_set_background_color(&s_data.day_label, GColorBlack);
  text_layer_set_text_color(&s_data.day_label, GColorWhite);
//  GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  text_layer_set_font(&s_data.day_label, norm18);

  layer_add_child(&s_data.date_layer, &s_data.day_label.layer);

  // init num
  text_layer_init(&s_data.num_label, GRect(73, 114, 18, 20));

  text_layer_set_text(&s_data.num_label, s_data.num_buffer);
  text_layer_set_background_color(&s_data.num_label, GColorBlack);
  text_layer_set_text_color(&s_data.num_label, GColorWhite);
  GFont bold18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  text_layer_set_font(&s_data.num_label, bold18);

  layer_add_child(&s_data.date_layer, &s_data.num_label.layer);

  // init hands
  layer_init(&s_data.hands_layer, s_data.simple_bg_layer.frame);
  s_data.hands_layer.update_proc = &hands_update_proc;
  layer_add_child(&s_data.window.layer, &s_data.hands_layer);

}  // feature_analog_init()

