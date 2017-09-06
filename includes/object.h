#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../utility/vec3.h"
#include "../utility/ray.h"
#include "material.h"

//class Material;

struct HitRecord
{
    float t;
    point3 p;
    vec3 normal;
    Material *mat;
};

class Object {

    protected:
        point3 origin;
        Material *material;

    public:
        virtual bool hit( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ ) const = 0;

        void print();
};




void Object::print(){
    std::cout << "( " << origin.x() << ", " << origin.y() << ", "<< origin.z() << " )" << std::endl; 
}

#endif
