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
        float cosine_numerator;
        float cosine_denominator;
        vec3 l;

        // calculate de cosine between the normal vector and the camera ray
        // dot(N * D) = |N|*|D|*cosϴ
        cosine_numerator = dot( ht.normal, r_.get_direction() );       
        cosine_denominator = ht.normal.length() * r_.get_direction().length();

        // make the border
        if (fabs(cosine_numerator/cosine_denominator) <= fabs(cos(80 * PI / 180.0)))
        {               
            return rgb (0,0,0);             
        }

        // Generate the shades based on the lights
        for( int i = 0; i < Shader::world->lum_size; i++)
        {
            // calculate de cosine between the normal vector and the light ray
            l = (world->lum[i]->direction - r_.get_direction());
        	cosine_numerator = dot( ht.normal, l );
        	cosine_denominator = ht.normal.length() * l.length();

            // if the angle is between the first interval, than color the pixel with the first color
            if ((cosine_numerator/cosine_denominator) >= (cos(ht.mat->angles[0]* PI / 180.0)))
            {
                hue += ht.mat->gradient[0];
            }else 
            {
                // chech if the angle is between the others intervals
            	for (int j = ht.mat->angles.size()-1; j >= 1; --j)
            	{
                    // if the angle that the light hits the object is smaller than
                    // the angle of the desired angle
                    if( (cosine_numerator/cosine_denominator) >= (cos(ht.mat->angles[j]* PI / 180.0)) &&
                        (cosine_numerator/cosine_denominator) <= (cos(ht.mat->angles[j-1]* PI / 180.0)) )
                    {
        	      		hue += ht.mat->gradient[j];
        	    	}
            	}
            }
        }
        return hue/(Shader::world->lum_size);
    	
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif