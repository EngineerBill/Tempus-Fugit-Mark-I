/* -------------------------------------------------------------------------------------- *\

                      page_about.c

	Implements Tempus Fugit Main Program About window

	Called from :
					- Main Program menu

----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/18/13
	Last Update:		8/18/13
   ----------------------------------------------------------------

TODO:
		- Need to clean up URL (crruently wraps)
		- TBD: should this be blog or website?

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
//#include "resource_ids.auto.h"	// provides hook to program resources

#include "TempusFugit.h"			// pull in global declares
#include "page_about.h"			// pull in local declares

// Local Window Layers structure
static struct TempusFugitAboutData {
//	BmpContainer logo_container;	// logo image container

	TextLayer title_layer;			// About window text layers
	TextLayer copyright_layer;		//
	TextLayer publisher_layer;		//
	TextLayer contact_layer;		//
} about_data;

// Logo Container
static BmpContainer logo_container;	// logo image container



static Window about_window;			// Module Window 

static void handle_about_appear();	// forward references
static void handle_about_disappear();

// --------------------------------------------------------
//			page_about_show_page()
//
//         Display About window
// --------------------------------------------------------
void page_about_show_page(){
// ---------------------------------------
//    activate show window
// ---------------------------------------

	window_stack_push(&about_window, true);

}  // page_about_show_page()

void page_about_init() {
	window_init(&about_window, "About...");

	window_set_window_handlers(&about_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_about_appear,
        .disappear = (WindowHandler)handle_about_disappear,  // we'll remove window when we're done
//		.load = handle_about_load,
//		.unload = handle_about_unload,
    });

//	window_set_fullscreen(&about_window, true);		// remove top bar and replace with debug layer 
	window_set_background_color(&about_window, GColorBlack);

// ---------------------------------------
//   now initialize text layers
// ---------------------------------------
//	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
//	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);


// set up logo
	bmp_init_container(IMAGE_RESOURCE_IDS[0], &logo_container);
	layer_set_frame(&logo_container.layer.layer, GRect(5,15,40,46));
	layer_add_child(&about_window.layer, &logo_container.layer.layer);

// set up title layer
	text_layer_init(&about_data.title_layer, GRect (54, 22, 88, 32));
	text_layer_set_text_alignment(&about_data.title_layer, GTextAlignmentCenter);
	text_layer_set_text(&about_data.title_layer, "Tempus Fugit - Mark I");
	text_layer_set_background_color(&about_data.title_layer, GColorBlack);
	text_layer_set_text_color(&about_data.title_layer, GColorWhite);
	text_layer_set_font(&about_data.title_layer, norm14);
	layer_add_child(&about_window.layer, &about_data.title_layer.layer);	

// set up copyright layer
	text_layer_init(&about_data.copyright_layer, GRect (27, 73, 90, 18));
	text_layer_set_text_alignment(&about_data.copyright_layer, GTextAlignmentCenter);
	text_layer_set_text(&about_data.copyright_layer, "(c) 2013");
	text_layer_set_background_color(&about_data.copyright_layer, GColorBlack);
	text_layer_set_text_color(&about_data.copyright_layer, GColorWhite);
	text_layer_set_font(&about_data.copyright_layer, norm14);
	layer_add_child(&about_window.layer, &about_data.copyright_layer.layer);	

// set up publisher layer
	text_layer_init(&about_data.publisher_layer, GRect (21, 93, 100, 34));
	text_layer_set_text_alignment(&about_data.publisher_layer, GTextAlignmentCenter);
	text_layer_set_text(&about_data.publisher_layer, "Steamchest Chronicles");
	text_layer_set_background_color(&about_data.publisher_layer, GColorBlack);
	text_layer_set_text_color(&about_data.publisher_layer, GColorWhite);
	text_layer_set_font(&about_data.publisher_layer, norm14);
	layer_add_child(&about_window.layer, &about_data.publisher_layer.layer);		

// set up contact layer
	text_layer_init(&about_data.contact_layer, GRect (2, 128, 141, 34));
	text_layer_set_text_alignment(&about_data.contact_layer, GTextAlignmentCenter);
//	text_layer_set_text(&about_data.contact_layer, "steamcab.wordpress.com");
	text_layer_set_text(&about_data.contact_layer, "www.steamchest.com");
	text_layer_set_background_color(&about_data.contact_layer, GColorBlack);
	text_layer_set_text_color(&about_data.contact_layer, GColorWhite);
	text_layer_set_font(&about_data.contact_layer, norm14);
	layer_add_child(&about_window.layer, &about_data.contact_layer.layer);


}  // page_about_init()

// --------------------------------------------------------
//		page_about_deinit()
//
//   remove window when we're done with it.
// --------------------------------------------------------
void page_about_deinit() {

	bmp_deinit_container(&logo_container);

}  // page_about_deinit()

// --------------------------------------------------------
//		handle_about_disappear()
//
//   remove window when we're done with it.
// --------------------------------------------------------
void handle_about_appear() {

// need to set "active window" here
//	tf_active_window = ABOUT_WINDOW;

	
}  // handle_about_disappear()

// --------------------------------------------------------
//		handle_about_disappear()
//
//   remove window when we're done with it.
// --------------------------------------------------------
void handle_about_disappear() {

//	tf_active_window = NO_WINDOW;		// assumes windows are removed before new ones added...

	
}  // handle_about_disappear()

