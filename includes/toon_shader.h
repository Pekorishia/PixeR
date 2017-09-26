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


//CONSIDERAR O RAIO DA LUZ!!!!!!!!!!!
rgb ToonShader::color(const Ray & r_, float t_min, float t_max, int depth_) const{
	HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
    	float cosseno1 = dot( ht.normal, r_.get_direction() );
    	float cosseno2 = ht.normal.length() * r_.get_direction().length();

        rgb hue = ht.mat->gradient[0];

    	for (int i = 1; i < ht.mat->angles.size(); ++i)
    	{
            // the cos of angle between the cam ray and the sphere normal 
            if (fabs(cosseno1/cosseno2) <= fabs(cos(80 * PI / 180.0))){
                return rgb (0,0,0);
            }
    		else if(fabs(cosseno1/cosseno2) >= fabs(cos(ht.mat->angles[i]* PI / 180.0)) &&
                    fabs(cosseno1/cosseno2) <= fabs(cos(ht.mat->angles[i - 1]* PI / 180.0)) ){
	      		return ht.mat->gradient[i];
	    	}
    	}
        return hue;
    	
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif