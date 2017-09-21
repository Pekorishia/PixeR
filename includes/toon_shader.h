#ifndef _ToonSHADER_H_
#define _ToonSHADER_H_

#include "shader.h" 

class ToonShader : public Shader
{
	public:
		ToonShader(Scene *world_){
			 Shader::world = world_;
		}

		 virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
	
};


rgb ToonShader::color(const Ray & r_, float t_min, float t_max, int depth_) const{
	HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
      return ht.mat->gradient[0];
    }

    // Else, dye the pixel with the background color
    return Shader::vertical_interpolation(r_, Shader::world->bg->lower_left, Shader::world->bg->top_left);
}

#endif