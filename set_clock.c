#include <os.h>
#include "time.h"
#include "draw.h"

enum {
    SETTING_HOUR,
    SETTING_MIN,
    SETTING_DAY,
    SETTING_MONTH,
    SETTING_YEAR,
    SETTING_MAX
};

static void display_time(unsigned state, struct time_t *tm) {
    char buffer[16];
    char *text;
    unsigned number;

    switch (state) {
        case SETTING_HOUR:
            number = tm->hour;
            text = "Hour";
            break;
        case SETTING_MIN:
            number = tm->min;
            text = "Mins";
            break;
        case SETTING_DAY:
            number = tm->day;
            text = "Day";
            break;
        case SETTING_MONTH:
            number = tm->mon;
            text = "Month";
            break;
        case SETTING_YEAR:
            number = tm->year;
            text = "Year";
            break;
        default:
            number = 0;
            text = "?";
            break;
    }

    sprintf(buffer, "%4u", number);
    draw_string_scaled(80, 90, 0, 7, buffer);
    sprintf(buffer, "%5s", text);
    draw_string_scaled(0, 120, 0, 2, buffer);

}

void set_clock() {
    unsigned state = 0;

    struct time_t tm = time_convert_decompose(time_current());

    memset(SCREEN_BASE_ADDRESS, 0xff, SCREEN_BYTES_SIZE);

    display_time(state, &tm);
    sleep(300);

    while (1) {
        if (isKeyPressed(KEY_NSPIRE_LEFT)) {
            state = state == 0 ? SETTING_MAX - 1 : state - 1;
        } else if (isKeyPressed(KEY_NSPIRE_RIGHT)) {
            state = (state + 1) % SETTING_MAX;
        } else if (isKeyPressed(KEY_NSPIRE_UP)) {
            switch (state) {
                case SETTING_HOUR:
                    tm.hour = (tm.hour + 1) % 24;
                    break;
                case SETTING_MIN:
                    tm.min = (tm.min + 1) % 60;
                    break;
                case SETTING_DAY:
                    tm.day = (tm.day + 1) % 32;
                    if (tm.day == 0) tm.day = 1;
                    break;
                case SETTING_MONTH:
                    tm.mon = ((tm.mon + 1) % 13);
                    if (tm.mon == 0) tm.mon = 1;
                    break;
                case SETTING_YEAR:
                    tm.year++;
                    break;
            }
        } else if (isKeyPressed(KEY_NSPIRE_DOWN)) {
            switch (state) {
                case SETTING_HOUR:
                    tm.hour = tm.hour == 0 ? 23 : tm.hour - 1;
                    break;
                case SETTING_MIN:
                    tm.min = tm.min == 0 ? 59 : tm.min - 1;
                    break;
                case SETTING_DAY:
                    tm.day = tm.day == 1 ? 31 : tm.day - 1;
                    break;
                case SETTING_MONTH:
                    tm.mon = tm.mon == 1 ? 12 : tm.hour - 1;
                    break;
                case SETTING_YEAR:
                    if (tm.year >= 1970) tm.year--;
                    break;
            }
        } else if (isKeyPressed(KEY_NSPIRE_ENTER)) {
            break;
        } else if (isKeyPressed(KEY_NSPIRE_ESC)) {
            goto noset;
        } else {
            continue;
        }
        display_time(state, &tm);
        sleep(200);
    }
    unsigned time = time_convert_compose(&tm);
    time_set(time);

    noset:
    return;
}
