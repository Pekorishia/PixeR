
#ifndef _Plane_H_
#define _Plane_H_

#include "object.h"
#include "material.h"
#include "triangle_object.h"

#define error 0.00001

class Plane : public Object {

    public:

        Plane(Material *m_, point3 v0_, point3 v1_, point3 v2_, point3 v3_, bool backface_cull_ = false)
        {
            Object::origin = point3 (0,0,0);
            Object::material = m_;
            t0 = new Triangle(m_, v0_, v1_, v2_);
            t1 = new Triangle(m_, v2_, v3_, v0_);
            v0 = v0_;
            v1 = v1_;
            v2 = v2_;
            v3 = v3_;
            //new Triangle(mat, point3 (v0_[0], v0[1], v0[2]), point3 (v1[0], v1[1], v1[2]), point3 (v2[0], v2[1], v2[2]));
            
            backface_cull = backface_cull_;
        }

        //=== Atributes
        bool backface_cull;
        point3 v0;
        point3 v1;
        point3 v2;
        point3 v3;

        Triangle *t0;
        Triangle *t1;

        //=== Access methods
        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};

/*  
 * Returns true if the ray hits the Plane  or false otherwise  
 */ 
bool Plane::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{

    vec3 e1, e2, h, s, q;
    float det,inv_det,u,v;
 
    // e1 = (v3 - v2);
    // e2 = (v0 - v2);

    // h = cross(r_.get_direction(), e2);

    // det = dot(e1, h);

    

    //     if (det > -error && det < error)
    //         return false;

    //     inv_det = 1.0/det;

    //     s =  r_.get_origin() - v2;

    //     u = inv_det * dot(s, h);

    //     if (u < 0.0 || u > 1.0)
    //         return false;

    //     q = cross(s, e1);

    //     v = inv_det * dot(r_.get_direction(), q);

    //     if (v < 0.0 || u + v > 1.0)
    //         return false;

    //     float t = inv_det * dot(e2, q);

    //     if (t > error && t <= t_max_) // ray intersection
    //     {
    //             ht_.t = t;
    //             ht_.p =  r_.point_at(t);
    //             ht_.normal =  unit_vector( cross(e1, e2) );  
    //             ht_.mat = Object::material;
    //         return true;
    //     }

 
    e1 = (v1 - v0);
    e2 = (v2 - v0);

    h = cross(r_.get_direction(), e2);

    det = dot(e1, h);

    

        if (det > -error && det < error)
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

        if (t > error && t <= t_max_) // ray intersection
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

#endif
