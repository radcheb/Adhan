/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_COORDINATES_H
#define C_COORDINATES_H

typedef struct {
    const double latitude;
    const double longitude;
} coordinates_t;

inline coordinates_t *new_coordinates(double *latitude, double *longitude){
    return (coordinates_t*)(latitude, longitude);
}

#endif //C_COORDINATES_H