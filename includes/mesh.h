#ifndef _Mesh_H_
#define _Mesh_H_

#include "object.h"
#include "material.h"
#include "triangle_object.h"
#include "cube.h"
#include "kdnode.h"

class Mesh : public Object {

    public:

        Cube* bbox; 

        KDNode* node = new KDNode();

        vector<Triangle*> triangles;

        Mesh();

        Mesh(Material *m_, vector<Triangle*> triangles_, Cube* bbox_)
        {
            Object::material = m_;
            triangles = triangles_;
            bbox = bbox_;
            node = node->build(triangles_, 0);

        }

        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};


bool Mesh::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
    // if(bbox->hit(r_, t_min_, t_max_, ht_)){
    //     for (int i = 0; i < triangles.size(); i++ ){
    //         if (triangles[i]->hit(r_, t_min_, t_max_, ht_) ) {
    //             return true;
    //         }
    //     }
    // }
    // return false;

    
    bool result = false;
    if(bbox->hit(r_, t_min_, t_max_, ht_)){
        result = node->hit(node, r_, t_min_, t_max_, ht_);
    }
    return result;
}

#endif