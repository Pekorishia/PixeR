#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../utility/vec3.h"
#include "../utility/ray.h"
//#include "material.h" 

class Material;

typedef struct
{
    float t;
    point3 p;
    vec3 normal;
    Material *mat;
} HitRecord;

class Object {

    protected:
        point3 origin; // the origin of the object.
        Material *material;

    public:
        //=== Alias
        typedef float real_type;

        //=== Access methods
        virtual bool hit( const Ray & r_, real_type t_min_, real_type t_max_, HitRecord & ht_ ) const = 0;
};

#endif
