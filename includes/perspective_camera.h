#ifndef _PERSPECTIVECAMERA_H_
#define _PERSPECTIVECAMERA_H_

#include "../utility/ray.h"

class PerspectiveCamera : public Camera
{
public:

    PerspectiveCamera( point3 e_, point3 d_, vec3 vup_, float fov_, float aspect_, float aperture_, float focus_dist_)
    {
        lens_radius = aperture_ / 2;
        vec3 u, v, w;

        float theta = fov_ * M_PI/180; // Radian transformation

        float half_height = tan(theta/2);
        float half_width = aspect_ * half_height;

        Camera::origin = e_; 

        w = unit_vector(e_ -  d_);
        u = unit_vector(cross(vup_, w));
        v = cross(w,u);

        Camera::lower_left_corner = e_ - half_width * focus_dist_* u - half_height * focus_dist_ * v - w * focus_dist_;
        Camera::horizontal = 2 * half_width * focus_dist_ * u; 
        Camera::vertical = 2 * half_height * focus_dist_ * v; 
    }	

    virtual Ray get_ray(float u_, float v_) const;
    vec3 random_in_unit_sphere() const;

    float lens_radius;
    vec3 u,v,w;
};

/*
 * Generates a random ray
 */
vec3 PerspectiveCamera::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);
    return p;
}

/*
 * Get the ray that hits the view plane on the coordinate (u,v)
 */
Ray PerspectiveCamera::get_ray(float u_, float v_) const
{
    vec3 rd = lens_radius * random_in_unit_sphere();
    vec3 offset = u * rd.x() + v * rd.y();

    point3 end_point = Camera::lower_left_corner + u_*Camera::horizontal + v_*Camera::vertical ;
    Ray r( Camera::origin * offset, end_point - origin - offset );
    return r;
}

#endif
