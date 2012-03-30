#include <os.h>
#include "charmap.h"


#define set_pixel_cx(fb, color) *((uint16_t*)(fb)) = color
#define set_pixel_grey_high(fb, color) *((uint8_t*)fb) = (color<<4) | ((*((uint8_t*)fb)) & 0x0f)
#define set_pixel_grey_low(fb, color) *((uint8_t*)fb) = (color & 0xf) | ((*((uint8_t*)fb)) & 0xf0)

#define SCREEN_PIXEL_SIZE_CX 2
#define SCREEN_PIXEL_SIZE_GREY 1

unsigned draw_color_cx[2] = {0xffff, 0x0} , draw_color_grey[2] = {0xff, 0x0};

static inline void draw_pixel_scaled(unsigned x, unsigned y, unsigned scale, unsigned bit) {
    //x *= scale;
    //y *= scale;
    if (x>=(SCREEN_WIDTH-scale) || y>=(SCREEN_HEIGHT-scale)) return;

    char *fb = SCREEN_BASE_ADDRESS;

    unsigned i,j;
    for (i=0; i<scale; i++) {
        for (j=0; j<scale; j++) {
            if (is_cx) {
                set_pixel_cx(fb + ((y+i) * SCREEN_WIDTH * SCREEN_PIXEL_SIZE_CX) + ((x+j) * SCREEN_PIXEL_SIZE_CX), draw_color_cx[bit&1]);
            }else{
                if ((x+j) % 2 == 0) {
                    set_pixel_grey_low(fb + (((y+i) * SCREEN_WIDTH * SCREEN_PIXEL_SIZE_GREY) + ((x+j) * SCREEN_PIXEL_SIZE_GREY))/2, draw_color_grey[bit&1]);
                }else{
                    set_pixel_grey_high(fb + (((y+i) * SCREEN_WIDTH * SCREEN_PIXEL_SIZE_GREY) + ((x+j) * SCREEN_PIXEL_SIZE_GREY))/2, draw_color_grey[bit&1]);
                }
            }
        }
    }
}

inline void draw_char_scaled(unsigned x, unsigned y, unsigned scale, unsigned char c) {
    unsigned i,j;
    for (i=0; i<CHAR_HEIGHT; i++) {
        for (j=0; j<CHAR_WIDTH; j++) {
            draw_pixel_scaled( x + (j*scale), y + (i*scale), scale, (charmap[c][i]>>(CHAR_WIDTH-j-1)) & 1);
        }
    }

}

void draw_string_scaled(unsigned x, unsigned y, unsigned line_spacing, unsigned scale, char * string) {
    while (*string) {
        draw_char_scaled(x, y, scale, *string);
        x += line_spacing;
        x += CHAR_WIDTH*scale;
        string++;
    }

}