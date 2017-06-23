//
// Created by stormcaster on 22/06/17.
//
#include <math.h>

#ifndef C_DOUBLE_UTILS_H
#define C_DOUBLE_UTILS_H

double normalize_with_bound(double value, double max) { return value - (max * (floor(value / max))); }

double unwind_angle(double value){return normalize_with_bound(value, 360);}

double closest_angle(double angle){
    if (angle >= -180 && angle <= 180) {
        return angle;
    }
    return angle - (360 * round(angle / 360));
}
#endif //C_DOUBLE_UTILS_H
