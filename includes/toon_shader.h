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
    HitRecord ht_s;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        rgb hue = rgb (0,0,0);
        vec3 l;
        float cosine_N_ray;
        float cosine_N_light;

        // calculate de cosine between the normal vector and the camera ray
        // dot(N * D) = |N|*|D|*cosϴ
        cosine_N_ray = dot( ht.normal, r_.get_direction() )/( ht.normal.length() * r_.get_direction().length());

        // make the border
        if (fabs(cosine_N_ray) <= fabs(cos(80 * PI / 180.0)))
        {               
            return rgb (0,0,0);             
        }

        // The origin for the shadow ray
        auto p = r_.get_origin() + ht.t * r_.get_direction();

        // get the size of the angles array
        auto angles_size = ht.mat->angles.size();

        // actual position of the picked color                    
        auto angle_picked_position = angles_size;

        // actual intensity of the picked light
        auto light_picked_intensity = 0;

        // verify if tihs pixel is a shadow
        auto shadow = false;

        // Generate the shades based on the lights
        for( int i = 0; i < Shader::world->lum_size; i++)
        {
            // If the light didn't hit anything before hiting the point, then color it
            if (! Shader::hit_anything( Ray(p, world->lum[i]->direction), 0.001f, std::numeric_limits<float>::infinity(), ht_s) )
            {
                // calculate de cosine between the normal vector and the light ray
                l = (world->lum[i]->direction - r_.get_direction());                
            	cosine_N_light = dot( ht.normal, l )/(ht.normal.length() * l.length());
                
                // get the intensity of this light
                auto light_intensity = world->lum[i]->direction.length();

                // chech if the angle is between the others intervals
            	for (int j = angles_size-1; j >= 0; --j)
            	{

                    // if the angle is between the first interval, than color the pixel with the first color
                    if (j==0 && (cosine_N_light) >= (cos(ht.mat->angles[0]* PI / 180.0)))
                    {
                        // if the actual 
                        if (angle_picked_position > j &&
                            light_picked_intensity < light_intensity)
                        { 
                            angle_picked_position = j;
                            light_picked_intensity = light_intensity;

                            hue = ht.mat->gradient[j]; 
                        }
                    }
                    // if the angle that the light hits the object is smaller than
                    // the angle of the desired angle
                    else if( cosine_N_light >= (cos(ht.mat->angles[j]* PI / 180.0)) &&
                        cosine_N_light <= (cos(ht.mat->angles[j-1]* PI / 180.0)) )
                    {
                        // if the actual 
                        if (angle_picked_position > j &&
                            light_picked_intensity < light_intensity)
                        { 
                            angle_picked_position = j;
                            light_picked_intensity = light_intensity;

                            hue = ht.mat->gradient[j]; 
                        }
        	    	}
            	}
            }
            else { shadow = true; }
        }
        if (shadow) {return hue/(Shader::world->lum_size * 2);}
        else { return hue/(Shader::world->lum_size);}    	
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif