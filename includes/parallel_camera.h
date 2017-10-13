#ifndef _ParallelCamera_H_
#define _ParallelCamera_H_

#include "../utility/ray.h"

class ParallelCamera : public Camera
{
public:

    ParallelCamera( point3 e_, point3 d_, vec3 vup_, float l_, float r_, float b_, float t_)
    {
        Camera::origin = e_; 

        w = unit_vector(e_ -  d_);
        u = unit_vector(cross(vup_, w));
        v = cross(w,u);

        Camera::lower_left_corner = point3 (l_, b_, e_.z());
        Camera::horizontal = vec3 (r_ - l_,0,0); 
        Camera::vertical = vec3 (0,t_ - b_,0); 
    }	

    vec3 u, v, w;

    virtual Ray get_ray(float u_, float v_) const;
       
};

/*
 * Get the ray that hits the view plane on the coordinate (u,v)
 */
Ray ParallelCamera::get_ray(float u_, float v_) const
{
    Ray r( Camera::lower_left_corner + u_*Camera::horizontal + v_*Camera::vertical, -w);
    return r;
}

#endif
