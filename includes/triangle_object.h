
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "material.h"

#define epsilon 0.00001

class Triangle : public Object {

    public:

        Triangle(Material *m_, point3 v0_, point3 v1_, point3 v2_, bool backface_cull_ = false)
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

    vec3 e1, e2, h, s, q;
    float det,inv_det,u,v;
 
    e1 = (v1 - v0);
    e2 = (v2 - v0);

    h = cross(r_.get_direction(), e2);

    det = dot(e1, h);

    if (backface_cull){
        if (det < epsilon)
            return false;

        s =  r_.get_origin() - v0;

        u = dot(s, h);

        if (u < 0.0 || u > det)
            return false;

        q = cross(s, e1);

        v = dot(r_.get_direction(), q);

        if (v < 0.0 || u + v > det)
            return false;

        float t = inv_det * dot(e2, q);

        inv_det = 1.0/det;

        t *=inv_det;
        u *=inv_det;
        v *=inv_det;

        if (t > epsilon) // ray intersection
        {
            ht_.t = t;
            ht_.p =  r_.point_at(t);
            ht_.normal =  unit_vector( cross(e1, e2) );  
            ht_.mat = Object::material;
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }
    else{

        if (det > -epsilon && det < epsilon)
            return false;

        inv_det = 1.0/det;

        s =  r_.get_origin() - v0;

        u = inv_det * dot(s, h);

        if (u < 0.0 || u > 1.0)
            return false;

        q = cross(s, e1);

        v = inv_det * dot(r_.get_direction(), q);

        if (v < 0.0 || u + v > 1.0)
            return false;

        float t = inv_det * dot(e2, q);

        if (t > epsilon) // ray intersection
        {
                ht_.t = t;
                ht_.p =  r_.point_at(t);
                ht_.normal =  unit_vector( cross(e1, e2) );  
                ht_.mat = Object::material;
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }
}

#endif
