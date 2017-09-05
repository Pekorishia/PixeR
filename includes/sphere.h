
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object.h"
#include "material.h" 

class Sphere : public Object {

    public:

        Sphere(Material *m_, point3 center_ = point3(), float rad_ = 0.0)
        {
            Object::origin = center_;
            Object::material = m_;
            radius = rad_;
        }

        float radius; // the radius of the of the sphere

        //=== Access methods
        virtual bool hit( const Ray & r_, real_type t_min_, real_type t_max_, HitRecord & ht_ ) const;
};

/*  
 * Returns true if the ray hits the sphere  or false otherwise  
 *  
 * TODO: only works with a 2D sphere  
 */ 
bool Sphere::hit ( const Ray & r_, real_type t_min_, real_type t_max_, HitRecord & ht_ ) const
{
     // First, manipulate the 1), 2) and 3) line to create the 4) equation   
    // 1) (point - center) * (point - center) - radius² = 0     
    // 2) point == ray  // 3) ray(t) = origin + direction*t     
    // 4) d*d*t² + 2(origin - center)*d*t + (origin - center)*(origin - center) - radius² = 0   
    
    // Second, calculate the a, b and c from the equation 4) (a*t² + b*t + c = 0)   
    auto oc = r_.get_origin() - Object::origin; // (origin - center)    
    
    auto a = utility::dot( r_.get_direction(), r_.get_direction() ); // d*d  
    auto b = 2 * utility::dot( oc, r_.get_direction() );             // 2(origin-center)*d
    auto c = utility::dot(oc, oc) - (radius * radius);               // (origin-center)*(origin-center) - radius²

    // Third, calculate the delta (b² - 4ac)    
    auto delta = b * b - 4 * a * c;     

    // Last, return the t component if the ray hit the sphere 
    if (delta >= 0){
        auto t = (-b - sqrt(delta))/(2*a);

        if (t < t_max_ && t > t_min_){
            ht_.t = t;
            ht_.p =  r_.point_at(t);
            ht_.normal = (ht_.p - Object::origin) / radius;

            return true;
        }   
    }

    return false; 
}

#endif
