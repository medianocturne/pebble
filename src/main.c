#include <pebble.h>
//for git-it
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;
static void update_time() {
  //Get a tm structure
  time_t temp = time(NULL);
  time_t temp2 = time(NULL);
  struct tm *tick_time = localtime(&temp);
  struct tm *diggityDate = localtime(&temp2);
  //Create a long lived buffer
  static char buffer[] = "00:00";
  static char date[] = "DAY - xx.xx\0";
  #define dateFormat = "%a - %D";
  



  
  
  //Write the current hours and minutes into buffer
    strftime(date, sizeof("DAY - xx.xx\0"), "%a - %m/%d", diggityDate);
  
  if(clock_is_24h_style() == true){
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  //Display on TextLayer
text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_layer, date);


}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}



///***
/// LOAD
///***

static void main_window_load(Window *window){
  window_set_background_color(window, GColorBlack);

s_date_layer = text_layer_create(GRect(2, 130, 140, 24));
text_layer_set_text_color(s_date_layer, GColorWhite);
text_layer_set_background_color(s_date_layer, GColorClear);


  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 50, 144, 168));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
 //Improve layout 
  static GFont s_time_font;
  static GFont s_date_font;
  

  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_karate_48));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_karate_24));
  text_layer_set_font(s_date_layer, s_date_font);  
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);


  
  
  //Add as child layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));


 
}

///***
/// UNLOAD
///***
static void main_window_unload(Window *window){
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  text_layer_destroy(s_date_layer);
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
