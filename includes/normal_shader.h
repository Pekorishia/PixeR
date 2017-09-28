#ifndef _NORMAL_H_
#define _NORMAL_H_

#include "shader.h" 

class NormalShader : public Shader 
{

    public:

        NormalShader(Scene *world_)
        {
            Shader::world = world_;
        }

        //=== Access methods
        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
};

/*
 * Returns the color of the point hitted by the ray based on the normal vector.
 */
rgb NormalShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
       
        auto unit_normal = utility::unit_vector(ht.normal);

        // Nomalizing the unit normal to only positive values
        auto color = 0.5 * (unit_normal + vec3(1,1,1));
            
        return color;
    }

    // Else, dye the pixel with the background color
    Shader::interpolation_biline(r_);
}


#endif
