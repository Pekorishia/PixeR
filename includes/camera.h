#ifndef _CAMERA_H_
#define _SCENE_H_

#include "../utility/ray.h"

class Camera
{
public:

    point3 lower_left_corner; // lower left corner of the view plane.
    vec3 horizontal; // Horizontal dimension of the view plane.
    vec3 vertical; // Vertical dimension of the view plane.
    point3 origin; // the camera's origin.

    Camera()
    {
        lower_left_corner = point3(-2, -1, -1);
        horizontal = vec3(4, 0, 0); 
        vertical = vec3(0, 2, 0); 
        origin = point3(0, 0, 0); 
    }

    Ray get_ray(float u, float v)
    {
        point3 end_point = lower_left_corner + u*horizontal + v*vertical ;
        Ray r( origin, end_point - origin );
        return r;
    }
    
};
#endif