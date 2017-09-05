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
        virtual rgb color(const Ray & r_, float t_min, float t_max) const;
        virtual rgb color1() const;
};

rgb NormalShader::color1() const
{
    return rgb(1,0,0);
}

rgb NormalShader::color( const Ray & r_, float t_min, float t_max) const
{
    /*HitRecord ht;

    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
       
        auto unit_normal = utility::unit_vector(ht.normal);

        // Nomalizing the unit normal to only positive values
        auto color = 0.5 * (unit_normal + vec3(1,1,1));
            
        return color;
    }

    return Shader::vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
    */
    return rgb(1,0,0);
}


#endif
