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
} HitRecord; // Records the data from the last point hited by the camera ray 

class Object {

    protected:
        point3 origin; // the origin of the object.
        Material *material; // the material of the object.

    public:
        //=== Alias
        typedef float real_type;

        //=== Access methods
        virtual bool hit( const Ray & r_, real_type t_min_, real_type t_max_, HitRecord & ht_ ) const = 0;

        void print();
};

/*
 * Prints the origin of the object. Used for debugs
 */
void Object::print(){
    std::cout << "( " << origin.x() << ", " << origin.y() << ", "<< origin.z() << " )" << std::endl; 
}

#endif
