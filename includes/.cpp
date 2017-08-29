#ifndef _DIFUSE_H_
#define _DIFUSE_H_

#include "shader.h" 

class DifuseShader : public Shader 
{

    public:

        DifuseShader(Scene *world_)
        {
            Shader::world = world_;
        }

        //=== Access methods
        virtual rgb color(const Ray & r_, float t_min, float t_max);

    protected:

        vec3 random_in_unit_sphere();
};


vec3 DifuseShader::random_in_unit_sphere()
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);
    return p;
}

rgb DifuseShader::color( const Ray & r_, float t_min, float t_max)
{
    HitRecord ht;

    if ( hit_anything( r_, 0.0, t_max, ht) ) 
    {
        //Ray scattered_ray;
        vec3 target = ht.p + ht.normal + random_in_unit_sphere();
       // if( depth_ < 1)
        return 0.5 * color(Ray(ht.p, target - ht.p));
        //else
          //  return rgb(0,0,0);
        //return 0.5 * rgb(ht.normal.x() + 1, ht.normal.y() + 1, ht.normal.z() + 1);
    }

    //return vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
    vec3 unit_direction = unit_vector(r_.get_direction());
    float t = 0.5 *(unit_direction.y() + 1.0);
    return(1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}

#endif
