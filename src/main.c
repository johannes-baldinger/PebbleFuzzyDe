#include "pebble.h"
#include "num2words.h"

#define BUFFER_SIZE 24

static struct CommonWordsData {
  TextLayer *label ;
  TextLayer *label2;
  TextLayer *label3;
  TextLayer *label4;
  TextLayer *datum;
  TextLayer *conbatt;	
  Window *window;
  char buffer[BUFFER_SIZE],buffer2[BUFFER_SIZE],buffer3[BUFFER_SIZE],buffer4[BUFFER_SIZE];
  char date_text;
} s_data;


static void update_time(struct tm* t) {
	static char datum_text[] = "xx xx.xx.xxxx ************* (KW xx)";

	 strftime(datum_text,sizeof(datum_text), "%a %e.%m.%G \n(KW %V)", t);
  fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE);
  text_layer_set_text(s_data.label, s_data.buffer);
  text_layer_set_text(s_data.datum, datum_text);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
  GSize textsize = text_layer_get_content_size(s_data.label);
  
}

static void do_init(void) {
  s_data.window = window_create();
  const bool animated = true;
  window_stack_push(s_data.window, animated);

  window_set_background_color(s_data.window, GColorBlack);
  GFont font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BOLDFONT_32));
  GFont datumfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SMALLFONT_14));
  Layer *root_layer = window_get_root_layer(s_data.window);
  GRect frame = layer_get_frame(root_layer);

  s_data.label = text_layer_create(GRect(0, 0, frame.size.w, frame.size.h-30));
  s_data.datum = text_layer_create(GRect(0,frame.size.h-30,frame.size.w,30));
  text_layer_set_background_color(s_data.label, GColorBlack);
  text_layer_set_background_color(s_data.datum,GColorBlack);
  text_layer_set_text_color(s_data.label, GColorWhite);
  text_layer_set_text_color(s_data.datum,GColorWhite);
  text_layer_set_font(s_data.label, font);
  text_layer_set_font(s_data.datum,datumfont);
  text_layer_set_text_alignment(s_data.label, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_data.label, GTextAlignmentLeft);
  layer_add_child(root_layer, text_layer_get_layer(s_data.label));
  layer_add_child(root_layer, text_layer_get_layer(s_data.datum));
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);

  tick_timer_service_subscribe(SECOND_UNIT, &handle_minute_tick);
}

static void do_deinit(void) {
  window_destroy(s_data.window);
  text_layer_destroy(s_data.label);
  text_layer_destroy(s_data.datum);
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}