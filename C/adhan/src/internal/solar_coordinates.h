/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_SOLAR_COORDINATES_H
#define C_SOLAR_COORDINATES_H

typedef struct {
    double declination;
    double rightAscension;
    double apparentSiderealTime;
} solar_coordinates_t;

inline solar_coordinates_t *new_solar_coordinates(double julianDay);

#endif //C_SOLAR_COORDINATES_H
