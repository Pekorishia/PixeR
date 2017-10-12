#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../utility/ray.h"

class Camera
{
public:

    point3 lower_left_corner; // lower left corner of the view plane.
    vec3 horizontal; // Horizontal dimension of the view plane.
    vec3 vertical; // Vertical dimension of the view plane.
    point3 origin; // the camera's origin.

    virtual Ray get_ray(float u_, float v_) const = 0;
       
};


#endif
