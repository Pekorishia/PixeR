
#ifndef _Cube_H_
#define _Cube_H_

#include "object.h"
#include "material.h"
#include "triangle_object.h"
#include <math.h> 

#define error 0.00001

class Cube : public Object {

    public:

        Cube();

        Cube(Material *m_, point3 min_, point3 max_)
        {
            Object::origin = point3 (0,0,0);
            Object::material = m_;
            mini = min_;
            maxi = max_;
        }

        //=== Atributes
        point3 maxi;
        point3 mini;

        //=== Access methods

        Cube* wrap(Cube* node, Cube* bbox_tri)const;

        virtual bool hit( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const;
};

Cube* Cube::wrap(Cube* node, Cube* bbox_tri)const{

    if (node->mini.x() < bbox_tri->mini.x())
    {
        node->mini[0] = bbox_tri->mini.x();
    }
    if (node->mini.y() < bbox_tri->mini.y())
    {
        node->mini[1] = bbox_tri->mini.y();
    }
    if (node->mini.z() < bbox_tri->mini.z())
    {
        node->mini[2] = bbox_tri->mini.z();
    }

    if (node->maxi.x() > bbox_tri->maxi.x())
    {
        node->maxi[0] = bbox_tri->maxi.x();
    }
    if (node->maxi.y() > bbox_tri->maxi.y())
    {
        node->maxi[1] = bbox_tri->maxi.y();
    }
    if (node->maxi.z() > bbox_tri->maxi.z())
    {
        node->maxi[2] = bbox_tri->maxi.z();
    }
    return node;
}

/*  
 * Returns true if the ray hits the Cube  or false otherwise  
 */ 
bool Cube::hit ( const Ray & r_, float  t_min_, float  t_max_, HitRecord & ht_ ) const
{
    float tymin = (mini.y() - r_.get_origin().y()) / r_.get_direction().y(); 
    float tymax = (maxi.y() - r_.get_origin().y()) / r_.get_direction().y();

    double tmin = -INFINITY, tmax = INFINITY;

    for (int i = 0; i < 3; ++i) {
        double t1 = (mini[i] - r_.get_origin()[i]) / r_.get_direction()[i];
        double t2 = (maxi[i] - r_.get_origin()[i]) / r_.get_direction()[i];
 
        tmin = max(tmin, min(t1, t2));
        tmax = min(tmax, max(t1, t2));
    }

    if (tmax > max(tmin, 0.0)){
        float t =tmin;
        ht_.t = tmin;
        ht_.p =  r_.point_at(t);

        point3 c = (mini + maxi) * 0.5;
        vec3 p = ht_.p - c;
        point3 d = (mini - maxi) * 0.5;
        float bias = 1.000001;

        vec3 normal= vec3(
                p.x() / abs(d.x()) * bias,
                p.y() / abs(d.y()) * bias,
                p.z() / abs(d.z()) * bias
               );

        ht_.normal =  unit_vector( normal );  
        ht_.mat = Object::material;
        return true;
    }
 
    
        return false;

}

#endif
