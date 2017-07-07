/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_PRAYER_H
#define C_PRAYER_H

typedef enum {
    NONE,
    FAJR,
    SUNRISE,
    DHUHR,
    ASR,
    MAGHRIB,
    ISHA
} prayer_t;

static inline prayer_t* init_prayer(prayer_t prayer){
    prayer_t * rs = (prayer_t*)malloc(sizeof(prayer_t));
    *rs = prayer;
    return rs;
}
#endif //C_PRAYER_H
