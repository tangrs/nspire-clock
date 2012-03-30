#include <os.h>
#include "time.h"

#define YEAR_TO_DAYS(y) ((y)*365 + (y)/4 - (y)/100 + (y)/400)

unsigned time_convert_compose(struct time_t *tm) {
    unsigned time = 0;
    unsigned leap_days;

    time += 31536000*(tm->year - 1970);
    leap_days = (tm->year - 1972)/4 + 1;

    if (tm->year % 4 == 0) {
        if (tm->mon <= 2) leap_days--;
    }

    time += leap_days*86400;
    switch (tm->mon) {
        case 1:
            break;
        case 2:
            time += 2678400;
            break;
        case 3:
            time += 5097600;
            break;
        case 4:
            time += 7776000;
            break;
        case 5:
            time += 10368000;
            break;
        case 6:
            time += 13046400;
            break;
        case 7:
            time += 15638400;
            break;
        case 8:
            time += 18316800;
            break;
        case 9:
            time += 20995200;
            break;
        case 10:
            time += 23587200;
            break;
        case 11:
            time += 26265600;
            break;
        case 12:
            time += 28857600;
            break;
        default:
            break;
    }

    time += (tm->day-1) * 86400;
    time += tm->hour * 3600;
    time += tm->min * 60;
    time += tm->sec;

    return time;
}

/* adapted from http://stackoverflow.com/a/1275638/268025 */
struct time_t time_convert_decompose(unsigned unixtime)
{
    struct time_t tm;

    /* First take out the hour/minutes/seconds - this part is easy. */

    tm.sec = unixtime % 60;
    unixtime /= 60;

    tm.min = unixtime % 60;
    unixtime /= 60;

    tm.hour = unixtime % 24;
    unixtime /= 24;

    /* unixtime is now days since 01/01/1970 UTC
     * Rebaseline to the Common Era */

    unixtime += 719499;

    /* Roll forward looking for the year.  This could be done more efficiently
     * but this will do.  We have to start at 1969 because the year we calculate here
     * runs from March - so January and February 1970 will come out as 1969 here.
     */
    for (tm.year = 1969; unixtime > YEAR_TO_DAYS(tm.year + 1) + 30; tm.year++);

    /* OK we have our "year", so subtract off the days accounted for by full years. */
    unixtime -= YEAR_TO_DAYS(tm.year);

    /* unixtime is now number of days we are into the year (remembering that March 1
     * is the first day of the "year" still). */

    /* Roll forward looking for the month.  1 = March through to 12 = February. */
    for (tm.mon = 1; tm.mon < 12 && unixtime > 367*(tm.mon+1)/12; tm.mon++);

    /* Subtract off the days accounted for by full months */
    unixtime -= 367*tm.mon/12;

    /* unixtime is now number of days we are into the month */

    /* Adjust the month/year so that 1 = January, and years start where we
     * usually expect them to. */
    tm.mon += 2;
    if (tm.mon > 12)
    {
        tm.mon -= 12;
        tm.year++;
    }

    tm.day = unixtime;

    return tm;
}

inline unsigned time_current() {
    return *(volatile unsigned*)0x90090000;
}

inline void time_set(unsigned time) {
    *(volatile unsigned*)0x90090008 = time;
}

void time_wait_sec() {
    unsigned time = time_current();
    while (time == time_current()) {
        idle();
    }
}