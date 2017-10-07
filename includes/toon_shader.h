#ifndef _ToonSHADER_H_
#define _ToonSHADER_H_

#include "shader.h"

#define PI 3.14159265
#define border_angle fabs(cos(80 * PI / 180.0))

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
        if (fabs(cosine_N_ray) <= border_angle)
        {               
            return rgb (0,0,0);             
        }

        // get the size of the angles array
        auto angles_size = ht.mat->angles.size();

        // actual position of the picked color                    
        auto angle_picked_position = angles_size;

        // verify if this pixel is a shadow
        auto shadow = false;

        // count the number of shadows that this pixel has
        auto shadow_count = 1;

        // Generate the shades based on the lights
        for( int i = 0; i < Shader::world->lum_size; i++)
        {
            // If the light didn't hit anything before hiting the point, then color it
            if (! Shader::hit_anything( Ray(ht.p, world->lum[i]->direction), 0.001f, std::numeric_limits<float>::infinity(), ht_s) )
            {
                // calculate de cosine between the normal vector and the light ray
                l = (world->lum[i]->direction - r_.get_direction());                
            	cosine_N_light = dot( ht.normal, l )/(ht.normal.length() * l.length());
                
                // chech if the angle is between the others intervals
            	for (int j = angles_size-1; j >= 0; --j)
            	{

                    // if the angle is between the first interval, than color the pixel with the first color
                    if (j==0 && (cosine_N_light) >= ht.mat->angles[0])
                    {
                        // if the actual light is stronger and the angle is the lower one
                        if (angle_picked_position > j )
                        { 
                            angle_picked_position = j;

                            hue = ht.mat->gradient[j]; 
                        }
                    }
                    // if the angle that the light hits the object is smaller than
                    // the angle of the desired angle
                    else if( cosine_N_light >= ht.mat->angles[j] &&
                             cosine_N_light <= ht.mat->angles[j-1] )
                    {
                        // if the actual light is stronger and the angle is the lower one
                        if (angle_picked_position > j )
                        { 
                            angle_picked_position = j;

                            hue = ht.mat->gradient[j]; 
                        }
        	    	}
            	}
            }
            else { 
                shadow = true;
                shadow_count++;
            }
        }
        if (shadow) {return hue/(Shader::world->lum_size * shadow_count);}
        else { return hue/(Shader::world->lum_size);}    	
    }

    // Else, dye the pixel with the background color
    return Shader::interpolation_biline(r_);
}

#endif