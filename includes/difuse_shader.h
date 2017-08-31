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
        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;

};


rgb DifuseShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        /*vec3 p_ = random_in_unit_sphere();
        vec3 target = ht.p + ht.normal + p_;
        return 0.5 * this->color(Ray(ht.p, target - ht.p), t_min, t_max, 0);*/

        rgb attenuation;
        Ray scattered_ray;
        
        if (depth_ < 10 and ht.mat->scartter(r_, ht, attenuation, scattered_ray))
            return attenuation * this->color(scattered_ray, t_min, t_max, depth_+1);
        else
            return rgb(0,0,0);


    }

    return Shader::vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));

}

#endif
