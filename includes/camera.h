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

    Camera( point3 llc_, vec3 horizontal_, vec3 vertical_, point3 origin_)
    {
        lower_left_corner = llc_;
        horizontal = horizontal_; 
        vertical = vertical_; 
        origin = origin_; 
    }	

    Ray get_ray(float u, float v);
       
};

/*
 * Get the ray that hits the view plane on the coordinate (u,v)
 */
Ray Camera::get_ray(float u, float v)
{
    point3 end_point = lower_left_corner + u*horizontal + v*vertical ;
    Ray r( origin, end_point - origin );
    return r;
}

#endif
