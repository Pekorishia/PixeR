#ifndef _BLINNPHONG_H_
#define _BLINNPHONG_H_

#include <math.h>  

#include "shader.h" 

class BlinnphongShader : public Shader
{
	public:

		BlinnphongShader(Scene *world_)
        {
            Shader::world = world_;
        }

        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
	
};

rgb BlinnphongShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    { 
        rgb ip(0,0,0);

        vec3 r_unit = unit_vector(r_.get_direction());

        // The ray that goes back to the camera
        auto v = vec3( r_unit.x() * -1,  r_unit.y() * -1, r_unit.z() * -1);

        // Ambient light
        // ip += ht.mat->ka * Shader::world->ambientLight;

        for( int i = 0; i < Shader::world->lum_size; i++){

            // Light direction normalized
            auto l = unit_vector(world->lum[i]->direction - ht.normal);

            // Difuse component
            // ip += ht.mat->albedo * dot(l, ht.normal) * world->lum[i]->intensity;
            
            // Unit vector in the direction of ideal specular reflection: (2N * L)N - L
            auto r = dot (2 * ht.normal, l) * ht.normal - l;

            // Specular component
            ip += ht.mat->ks * pow ( dot( r, v ) , ht.mat->alpha ) * world->lum[i]->intensity;
        }

        // Remove any misscalculation to the [0;1] range
        if (ip.r() < 0){
            ip = rgb (0, ip.g(), ip.b());
        }
        if (ip.g() < 0){
            ip = rgb (ip.r(), 0, ip.b());
        }
        if (ip.b() < 0){
            ip = rgb (ip.r(), ip.g(), 0);
        }

        return ip;
    }

    // Else, dye the pixel with the background color
    return Shader::vertical_interpolation(r_, Shader::world->bg->lower_left, Shader::world->bg->top_left);
}

#endif