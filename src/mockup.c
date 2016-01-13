#include "pebble.h"

  
// Set the maximum no of images - this should be the number of images you have in the resources
static int max_images = 2;
 
// Array to hold the resource ID numbers - need to add each of the IDENTIFIER names you gave to each resource
const int MOCKUP_IMAGE_ID[] = {
  RESOURCE_ID_IMAGE_MOCKUP_1,
  RESOURCE_ID_IMAGE_MOCKUP_2
};


// Integer to hold the number of image we are currently displaying
static int image_no = 0;

// Pointer to main window and layer
static Window *s_main_window;
static Layer *s_main_window_layer;

// Pointer to mockup image and layer
static GBitmap *s_mockup_image;
static BitmapLayer *s_mockup_layer;


static void change_image(){

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "IMAGE: %d", image_no);
  
  bitmap_layer_set_bitmap(s_mockup_layer, NULL);
  gbitmap_destroy(s_mockup_image);
  
  s_mockup_image = gbitmap_create_with_resource(MOCKUP_IMAGE_ID[image_no]);
  
  bitmap_layer_set_bitmap(s_mockup_layer, s_mockup_image);
    
}


// Handle the button clicks
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

  // Nothing for select to do - could make it display image no
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  // decrease to show previous image
  image_no -= 1;
  
  // Check if we are at minimum and loop round by setting to max_images - 1
  if (image_no < 0){
    image_no = max_images - 1;
  }
  
  // Call the function that switches images and displays it on the layer
  change_image();
}


static void down_click_handler(ClickRecognizerRef recognizer, void *context) {  
  
  // increase for next image
  image_no += 1;
  
  // check if we are at maximum and loop round by setting to zero
  if (image_no > max_images - 1){
    image_no = 0;
  }
  
  // Call the function that switches images and displays it on the layer
  change_image();
  
}

static void click_config_provider(void *context) {
  
    // Setup each of the click providers
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}




// WINDOW LOAD
static void main_window_load(Window *window) {

  // Get the root window layer
  s_main_window_layer = window_get_root_layer(s_main_window);

  // Get the size of the main window - as size now different for Rect or Round
  GRect s_main_window_layer_frame = layer_get_frame(s_main_window_layer);
  
  // Create the layer that will hold the mockup bitmap image
  s_mockup_layer = bitmap_layer_create(s_main_window_layer_frame);

  // Set the bitmap layer to align the image in the center - useful if using the same 144x168 rect image on round
  bitmap_layer_set_alignment(s_mockup_layer, GAlignCenter);
  
  // Add the mockup window to the main window
  layer_add_child(s_main_window_layer, bitmap_layer_get_layer(s_mockup_layer));
  
  // Load the first image
  change_image();
  
}


// WINDOW UNLOAD
static void main_window_unload(Window *window) {

  //Destroy the mockup layer and image
  layer_remove_from_parent(bitmap_layer_get_layer(s_mockup_layer));
  bitmap_layer_destroy(s_mockup_layer);
  gbitmap_destroy(s_mockup_image);
  s_mockup_image = NULL;
    
}


// INIT
static void init(void) {

  // Create main Window
  s_main_window = window_create();
    
  // Handle clicks
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  // Set handlers to manage the loading and unloading of elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
    
}


// DEINIT
static void deinit(void) {

  // Destroy the main window
  window_destroy(s_main_window);
  
}


// MAIN PROGRAM LOOP
int main(void) {
  init();
  app_event_loop();
  deinit();
}