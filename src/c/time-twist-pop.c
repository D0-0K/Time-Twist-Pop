#include <pebble.h>

static Layer  *layer;
static Window *s_main_window;
static GBitmap *s_bitmap;

uint8_t *bitmap_data;
int bytes_per_row;

// Set the correct screen height and width (checking for Pebble Time Round)
int HEIGHT = PBL_IF_RECT_ELSE(180,180);
int WIDTH = PBL_IF_RECT_ELSE(180,180);

// Set the height and width of the image
int IMAGE_HEIGHT = 180;
int IMAGE_WIDTH = 180;

GColor get_pixel_color(int x, int y){
	return (GColor){ .argb = bitmap_data[y*bytes_per_row + x] };
}

// Set the color of the pixel at (x,y) to "color"
void set_pixel_color(int x, int y, GColor color){
	bitmap_data[y*bytes_per_row + x] = color.argb;
}

void replace_colors(int pixel_width, int pixel_height, GColor old_color, GColor new_color){
	int max_y = pixel_height; //Use 28 for just the hat
	int max_x = pixel_width;
	
	for(int y = 0; y < max_y; y++){
		for(int x = 0; x < max_x; x++){
			GColor pixel_color = get_pixel_color(x,y);
			if(gcolor_equal(pixel_color, old_color)){
			  set_pixel_color(x, y, new_color);
			}
		}
	}
}

void draw(Layer *layer, GContext *ctx) {
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    graphics_draw_bitmap_in_rect(ctx, s_bitmap, GRect(WIDTH-IMAGE_WIDTH, HEIGHT-IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT));
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  layer = layer_create(layer_get_bounds(window_get_root_layer(s_main_window)));

	layer_set_update_proc(layer, draw);
	layer_add_child(window_get_root_layer(s_main_window), layer);

  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_Background);

  bitmap_data = gbitmap_get_data(s_bitmap);
  bytes_per_row = gbitmap_get_bytes_per_row(s_bitmap);

	replace_colors(IMAGE_WIDTH, IMAGE_HEIGHT, GColorWhite, GColorBlue);
	replace_colors(IMAGE_WIDTH, IMAGE_HEIGHT, GColorLightGray, GColorMintGreen);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

