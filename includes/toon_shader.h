#ifndef _ToonSHADER_H_
#define _ToonSHADER_H_

#include "shader.h"
#include <math.h> 

#define PI 3.14159265

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
        rgb hue = rgb (0,0,0);
        float cosseno1;
        float cosseno2;
        vec3 l;

        //for( int i = 0; i < Shader::world->lum_size; i++){
            l = (world->lum[0]->direction - r_.get_direction());
        	cosseno1 = dot( ht.normal, l );
        	cosseno2 = ht.normal.length() * l.length();

        	for (int j = ht.mat->angles.size()-1; j >= 0; --j)
        	{
                if((cosseno1/cosseno2) >= (cos(ht.mat->angles[j]* PI / 180.0))  ){
    	      		hue = ht.mat->gradient[j];
    	    	}
        	}
        //}
        return hue;
    	
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif