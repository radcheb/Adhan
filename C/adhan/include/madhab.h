/*
 * Created by stormcaster on 22/06/17.
 */


#include "shadow.h"

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

static inline shadow_length getShadowLength(madhab_t madhab) {
        switch(madhab){
            case SHAFI:
                return SINGLE;
            case HANAFI:
                return DOUBLE;
            default:
                return SINGLE;
        }
}

static char *get_madhab_name(madhab_t madhab){
    switch (madhab){
        case SHAFI:
            return "Shafi";
        case HANAFI:
            return "hanafi";
    }
}
#endif //C_MADHAB_H
