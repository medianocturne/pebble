#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_bitmap_layer;
static GFont s_time_font;
static void update_time() {
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Create a long lived buffer
  static char buffer[] = "00:00";
  
  //Write the current hours and minutes into buffer
  if(clock_is_24h_style() == true){
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  //Display on TextLayer
text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}



static void main_window_load(Window *window){
  window_set_background_color(window, GColorBlack);

  
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 168));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
 //Improve layout 
  static GFont s_time_font;

  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_karate_48));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  

  
  
  //Add as child layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

 
}


static void main_window_unload(Window *window){
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
  bitmap_layer_destroy(s_bitmap_layer);

}


static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);

tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  update_time();
}

static void deinit () {
  window_destroy(s_main_window);
  
}

int main (void) {
  init();
  app_event_loop();
  deinit();
}