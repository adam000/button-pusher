#include <pebble.h>
#include <stdio.h>

Window *window;
TextLayer *text_layer;
int numPresses = 0;
char* initialText = "Push the up / down buttons. Middle button resets";
bool backButtonPressed = false;

// TODO clean this up
void config_provider(Window *window);

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 50, 144, 154));
   
   // Initialize click handlers
   window_set_click_config_provider(window, (ClickConfigProvider)config_provider);
  
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, initialText);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}

void down_single_click_handler(ClickRecognizerRef recognizer, void* context) {
   size_t len = sizeof(char) * 32;
   char* str = malloc(len);

   numPresses--;
   snprintf(str, len, "Hi, I'm a Pebble! %d", numPresses);
   text_layer_set_text(text_layer, str);
   
   free(str);
   backButtonPressed = false;
}

void up_single_click_handler(ClickRecognizerRef recognizer, void* context) {
   size_t len = sizeof(char) * 32;
   char* str = malloc(len);

   numPresses++;
   snprintf(str, len, "Hi, I'm a Pebble! %d", numPresses);
   text_layer_set_text(text_layer, str);
   
   free(str);
   backButtonPressed = false;
}

void mid_single_click_handler(ClickRecognizerRef recognizer, void* context) {
   text_layer_set_text(text_layer, initialText);
   numPresses = 0;
   backButtonPressed = false;
}

void back_single_click_handler(ClickRecognizerRef recognizer, void* context) {
   if (backButtonPressed) {
      window_stack_pop(true);
   }
   text_layer_set_text(text_layer, "Press back again to quit");
   backButtonPressed = true;
}

void config_provider(Window *window) {
   window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
   window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
   window_single_click_subscribe(BUTTON_ID_SELECT, mid_single_click_handler);
   window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler);
}