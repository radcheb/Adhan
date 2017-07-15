/*
 * Created by stormcaster on 22/06/17.
 */


#ifndef C_COORDINATES_H
#define C_COORDINATES_H

typedef struct {
    double latitude;
    double longitude;
} coordinates_t;

static inline void new_coordinates(coordinates_t *cord, double latitude, double longitude){
  cord->latitude = latitude;
  cord->longitude = longitude;
//    return (coordinates_t*)(latitude, longitude);
}

#endif //C_COORDINATES_H
