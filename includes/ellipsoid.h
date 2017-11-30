
#ifndef _Ellipsoid_H_
#define _Ellipsoid_H_

#include "object.h"
#include "material.h"

#define EPSILON 0.00001

class Ellipsoid : public Object {

    public:

        Ellipsoid(Material *m_, point3 center_, point3 size_)
        {
            Object::origin = center_;
            Object::material = m_;

            center = center_;
            size = size_;
        }

        //=== Atributes

        point3 center;
        point3 size;

        //=== Access methods
        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};

/*  
 * Returns true if the ray hits the Ellipsoid  or false otherwise  
 */ 
bool Ellipsoid::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{

    vec3 O_C = r_.get_origin() - center;
    vec3 dir = r_.get_direction();
    dir = unit_vector(dir);

    vec3 dir_ = r_.get_direction();


    float a = ((dir[0]*dir[0])/(size[0]*size[0]))
            + ((dir[1]*dir[1])/(size[1]*size[1]))
            + ((dir[2]*dir[2])/(size[2]*size[2]));

    float b = ((2.f*O_C[0]*dir[0])/(size[0]*size[0]))
            + ((2.f*O_C[1]*dir[1])/(size[1]*size[1]))
            + ((2.f*O_C[2]*dir[2])/(size[2]*size[2]));

    float c = ((O_C[0]*O_C[0])/(size[0]*size[0]))
            + ((O_C[1]*O_C[1])/(size[1]*size[1]))
            + ((O_C[2]*O_C[2])/(size[2]*size[2]))
            - 1.0f;

    float delta = ((b*b)-(4.f*a*c));

    if ( delta<0.f || a==0.f || b==0.f || c==0.f )
        return false;

    delta = sqrt(delta);


    float t1 = (-b+delta)/(2.f*a);
    float t2 = (-b-delta)/(2.f*a);

    if( t1<=EPSILON && t2<=EPSILON ) 
        return false; // both intersections are behind the ray origin



    bool back = (t1<=EPSILON || t2<=EPSILON); // If only one intersection (t>0) then we are inside the ellipsoid and the intersection is at the back of the ellipsoid
    
    float t=0.f;

    if( t1<=EPSILON )
       t = t2;
    else
       if( t2<=EPSILON )
          t = t1;
       else
          t=(t1<t2) ? t1 : t2;

    if( t<=EPSILON  || t >= t_max_) return false; // Too close to intersection


    vec3 intersection = r_.get_origin() + t*dir;
    vec3 normal = intersection - center  ;
    
    normal[0] = 2.f*normal[0]/(size[0] *size[0]);
    normal[1] = 2.f*normal[1]/(size[1] *size[1]);
    normal[2] = 2.f*normal[2]/(size[2] *size[2]);

    //normal *= (back) ? -1.f : 1.f;
    normal = unit_vector(normal);

    //std::cout << unit_vector(normal) << " ";

    ht_.t = t;
    ht_.p =  r_.point_at(t);
    ht_.normal =  normal;  
    ht_.mat = Object::material;
    return true;
}

#endif
