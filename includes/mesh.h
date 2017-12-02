#ifndef _Mesh_H_
#define _Mesh_H_

#include "object.h"
#include "material.h"
#include "triangle_object.h"

class Mesh : public Object {

    public:

        vector<Triangle*> triangles;

        Mesh(Material *m_, vector<Triangle*> triangles_)
        {
            Object::material = m_;
            triangles = triangles_;
        }

        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};


bool Mesh::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
    bool reached = false;
    // Check hit
    for (int i = 0; i < triangles.size(); i++ ){
        if (triangles[i]->hit(r_, t_min_, t_max_, ht_) ) {
            reached = true;
            break;
        }
    }
    return reached;
}
#endif