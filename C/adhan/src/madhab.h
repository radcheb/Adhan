/*
 * Created by stormcaster on 22/06/17.
 */


#include "internal/shadow.h"

#ifndef C_MADHAB_H
#define C_MADHAB_H

typedef enum {
    /**
     * Shafi Madhab
     */
            SHAFI,

    /**
     * Hanafi Madhab
     */
            HANAFI
} madhab_t;

shadow_length getShadowLength(madhab_t madhab) {
        switch(madhab){
            case SHAFI:
                return SINGLE;
            case HANAFI:
                return DOUBLE;
            default:
                return SINGLE;
        }
}
#endif //C_MADHAB_H
