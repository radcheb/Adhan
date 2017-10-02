/*
 * Created by stormcaster on 22/06/17.
 */


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

static char *get_high_latitude_rule_name(high_latitude_rule_t high_latitude_rule){
    switch (high_latitude_rule){
        case MIDDLE_OF_THE_NIGHT:
            return (char *) "Middle of the night";
        case SEVENTH_OF_THE_NIGHT:
            return (char *) "Seventh of the night";
        case TWILIGHT_ANGLE:
            return (char *) "Twilight angle";
        default:
            return (char *) "Unknow rule";
    }
}
#endif //C_HIGH_LATITUDE_RULE_H
