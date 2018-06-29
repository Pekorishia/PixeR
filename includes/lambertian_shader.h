#ifndef _DIFUSE_H_
#define _DIFUSE_H_

#include "shader.h"
#include "material.h" 
#include "lambertian_material.h"
#include "beerlaw_material.h"

class LambertianShader : public Shader 
{

    public:

        LambertianShader(Scene *world_)
        {
            Shader::world = world_;
        }

        //=== Access methods
        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;

    protected:
        vec3 random_in_unit_sphere() const;
};

/*
 * Generates a random ray
 */
vec3 LambertianShader::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);
    return p;
}

/*
 * Returns the color of the point hitted by the ray.
 */
rgb LambertianShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        Ray scattered_ray = r_;
        Ray scattered_ray2 = Ray(point3(0,0,0), vec3(0,0,0));
        float reflect_prob = 0.f;
        rgb attenuation = rgb(1,1,1);
        vec3 emitted = ht.mat->emitted(0,0,ht.p);
        if ( BeerLaw* b = dynamic_cast<BeerLaw*>(ht.mat)){
            if (depth_ > 0 ){
                if(ht.mat->scatter(r_, ht, attenuation, scattered_ray, reflect_prob, scattered_ray2) )
                    return emitted + attenuation * ( (reflect_prob * color(scattered_ray, t_min, t_max, depth_-1)) + ( (1-reflect_prob) * color(scattered_ray2, t_min, t_max, depth_-1)));
                else
                    return emitted + attenuation * color(scattered_ray, t_min, t_max, depth_-1);
            }
        }else{
            if (depth_ > 0 and ht.mat->scatter(r_, ht, attenuation, scattered_ray, reflect_prob, scattered_ray2) ){
                return emitted + attenuation * color(scattered_ray, t_min, t_max, depth_-1);
            }
        }
        
        return emitted;
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif
