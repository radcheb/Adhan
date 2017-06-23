/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_PRAYER_ADJUSTMENTS_H
#define C_PRAYER_ADJUSTMENTS_H

#include <stdlib.h>

typedef struct {
    /**
     * Fajr offset in minutes
     */
    int fajr;
    /**
     * Sunrise offset in minutes
     */
    int sunrise;
    /**
     * Dhuhr offset in minutes
     */
    int dhuhr;
    /**
     * Asr offset in minutes
     */
    int asr;
    /**
     * Maghrib offset in minutes
     */
    int maghrib;
    /**
     * Isha offset in minutes
     */
    int isha;
} prayer_adjustments_t;

inline prayer_adjustments_t* INIT_PRAYER_ADJUSTMENTS(){
    return (prayer_adjustments_t*)(0,0,0,0,0,0);
}

#endif //C_PRAYER_ADJUSTMENTS_H
