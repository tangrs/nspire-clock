#include <os.h>
#include "draw.h"
#include "time.h"
#include "set_clock.h"

#define TIME_AT_LEAST 1325397600 /* 1 / 1 / 2012 @ 0:0:0 EST */

static void display_time() {
    char buffer[64];
    unsigned current_time = time_current();
    struct time_t tm = time_convert_decompose(current_time);

    sprintf(buffer,"%02u%s%02u", tm.hour, current_time & 1 ? ":" : " ", tm.min);
    draw_string_scaled(4, 32, 0, 8, buffer);

    sprintf(buffer,"%2u/%2u/%4u", tm.day, tm.mon, tm.year);
    draw_string_scaled(72, 160, 0, 2, buffer);
}

int main() {
    unsigned cached_time = time_current();

    if (cached_time < TIME_AT_LEAST) {
        if (show_msgbox_2b("Clock","Your clock appears to be incorrect. Set time now?","Yes","No") == 1) set_clock();
    }

    memset(SCREEN_BASE_ADDRESS, 0xff, SCREEN_BYTES_SIZE);
    display_time();

    while (1) {
        if (isKeyPressed(KEY_NSPIRE_ESC)) {
            break;
        }
        if (isKeyPressed(KEY_NSPIRE_CTRL)) {
            if (isKeyPressed(KEY_NSPIRE_SHIFT)) {
                set_clock();
                memset(SCREEN_BASE_ADDRESS, 0xff, SCREEN_BYTES_SIZE);
            }
            if (isKeyPressed(KEY_NSPIRE_ENTER)) {
                struct time_t temp = time_convert_decompose(time_current());
                temp.sec = 0;
                temp.min++;
                time_set(time_convert_compose(&temp));
                sleep(1500);
            }
            if (isKeyPressed(KEY_NSPIRE_PLUS)) {
                if (*(volatile unsigned*)0x900F0020 < 0x95) {
                    (*(volatile unsigned*)0x900F0020)++;
                }
                sleep(100);
            }
            if (isKeyPressed(KEY_NSPIRE_MINUS)) {
                if (*(volatile unsigned*)0x900F0020 > 0x6B) {
                    (*(volatile unsigned*)0x900F0020)--;
                }
                sleep(100);
            }
            if (isKeyPressed(KEY_NSPIRE_VAR)) {
                draw_color_grey[0] = 0xff;
                draw_color_grey[1] = 0x0;
                draw_color_cx[0] = 0xffff;
                draw_color_cx[1] = 0x0;
                memset(SCREEN_BASE_ADDRESS, 0xff, SCREEN_BYTES_SIZE);
            }
            if (isKeyPressed(KEY_NSPIRE_DEL)) {
                draw_color_grey[0] = 0x0;
                draw_color_grey[1] = 0xff;
                draw_color_cx[0] = 0x0;
                draw_color_cx[1] = 0xffff;
                memset(SCREEN_BASE_ADDRESS, 0x00, SCREEN_BYTES_SIZE);
            }
        }
        if (cached_time != time_current()) {
            display_time();
            cached_time = time_current();
        }
        idle();
    }

    return 0;
}

