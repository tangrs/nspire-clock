struct time_t {
    unsigned sec, min, hour, day, mon, year;
};

unsigned time_convert_compose(struct time_t *tm);
struct time_t time_convert_decompose(unsigned unixtime);

unsigned time_current();
void time_set(unsigned time);
void time_wait_sec();