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

static inline prayer_adjustments_t * INIT_PRAYER_ADJUSTMENTS(){
    prayer_adjustments_t  *prayer_adjustments = (prayer_adjustments_t *)malloc(sizeof(prayer_adjustments_t));
    prayer_adjustments->fajr = 0;
    prayer_adjustments->sunrise = 0;
    prayer_adjustments->dhuhr = 0;
    prayer_adjustments->asr = 0;
    prayer_adjustments->maghrib = 0;
    prayer_adjustments->isha = 0;
   return prayer_adjustments;
}

#endif //C_PRAYER_ADJUSTMENTS_H
