//
// Created by stormcaster on 22/06/17.
//

#ifndef C_HIGH_LATITUDE_RULE_H
#define C_HIGH_LATITUDE_RULE_H

typedef enum {
    /**
     * Fajr will never be earlier than the middle of the night, and Isha will never be later than
     * the middle of the night.
     */
            MIDDLE_OF_THE_NIGHT,

    /**
     * Fajr will never be earlier than the beginning of the last seventh of the night, and Isha will
     * never be later than the end of hte first seventh of the night.
     */
            SEVENTH_OF_THE_NIGHT,

    /**
     * Similar to {@link HighLatitudeRule#SEVENTH_OF_THE_NIGHT}, but instead of 1/7th, the faction
     * of the night used is fajrAngle / 60 and ishaAngle/60.
     */
            TWILIGHT_ANGLE
} high_latitude_rule_t;

#endif //C_HIGH_LATITUDE_RULE_H
