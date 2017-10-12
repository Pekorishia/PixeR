#ifndef _ParallelCamera_H_
#define _ParallelCamera_H_

#include "../utility/ray.h"

class ParallelCamera : public Camera
{
public:

    ParallelCamera( point3 e_, point3 d_, vec3 vup_, float fov_, float aspect_)
    {
        float theta = fov_ * M_PI/180; // Radian transformation

        float half_height = tan(theta/2);
        float half_width = aspect_ * half_height;

        Camera::origin = e_; 

        w = unit_vector(e_ -  d_);
        u = unit_vector(cross(vup_, w));
        v = cross(w,u);

        Camera::lower_left_corner = e_ - half_width * u - half_height * v - w;
        Camera::horizontal = 2 * half_width * u; 
        Camera::vertical = 2 * half_height * v; 
    }	

    vec3 u, v, w;

    virtual Ray get_ray(float u_, float v_) const;
       
};

/*
 * Get the ray that hits the view plane on the coordinate (u,v)
 */
Ray ParallelCamera::get_ray(float u_, float v_) const
{
    //point3 end_point = Camera::lower_left_corner + u*Camera::horizontal + v*Camera::vertical ;
    

    Ray r( Camera::origin + u_*u + v_*v, -w);
    return r;
}

#endif
