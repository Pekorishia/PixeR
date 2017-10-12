
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "material.h"

#define epsilon 0.00001

class Triangle : public Object {

    public:

        Triangle(Material *m_, point3 v0_, point3 v1_, point3 v2_, bool backface_cull_ = true)
        {
            Object::origin = point3 (0,0,0);
            Object::material = m_;

            v0 = v0_;
            v1 = v1_;
            v2 = v2_;
            backface_cull = backface_cull_;
        }

        //=== Atributes
        bool backface_cull;

        point3 v0;
        point3 v1;
        point3 v2;

        //=== Access methods
        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};

/*  
 * Returns true if the ray hits the Triangle  or false otherwise  
 */ 
bool Triangle::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
 
    auto e1 = (v1 - v0);
    auto e2 = (v2 - v0);

    // If the culling is activate don't show the backface of the triangle
    if (backface_cull){
        
        if (t < t_max_ && t > t_min_){
            ht_.t = t;
            ht_.p =  r_.point_at(t);
            ht_.normal =  unit_vector( cross(e1, e2) );  
            ht_.mat = Object::material;

            return true;
        }   
    }
    // Else if the culling isn't activated, show the backface of the triangle
    else {

        if (t < t_max_ && t > t_min_){
            ht_.t = t;
            ht_.p =  r_.point_at(t);
            ht_.normal =  unit_vector(cross (e1, e2) );  
            ht_.mat = Object::material;

            return true;
        }   
    }

    return false; 
}

#endif
