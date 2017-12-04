
#ifndef _Plane_H_
#define _Plane_H_

#include "object.h"
#include "material.h"
#include "triangle_object.h"

#define error 0.00001

class Plane : public Object {

    public:

        Plane();

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

    if (t0->hit(r_, t_min_, t_max_, ht_) || t1->hit(r_, t_min_, t_max_, ht_))
    {
        return true;
    }else{
        return false;
    }

}

#endif
