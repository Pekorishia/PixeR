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
    	float cosseno1 = dot( ht.normal, r_.get_direction() );
    	float cosseno2 = ht.normal.length() * r_.get_direction().length();
    	if(fabs(cosseno1/cosseno2) > 0.1f){
    		//std::cout << dot(ht.normal, r_.get_direction()) << " ";
      		return ht.mat->gradient[0];
    	}
    	else{
    		return rgb(0,0,0);
    	}
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif