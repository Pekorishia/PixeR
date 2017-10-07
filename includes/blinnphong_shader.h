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
    HitRecord ht_s;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    { 
        rgb ip(0,0,0);

        vec3 r_unit = unit_vector(r_.get_direction());

        // Generate the reflected ray
        vec3 reflected = r_.get_direction() - 2 * dot(r_.get_direction(), ht.normal) * ht.normal;

        // The ray that goes back to the camera
        auto v = unit_vector(vec3( r_unit.x(),  r_unit.y(), r_unit.z()) * vec3(-1,-1,-1));

        // Ambient light
        auto ambient = ht.mat->ka  * world->ambientLight;

        ip += ambient;

        for( int i = 0; i < Shader::world->lum_size; i++){

        	// If the light didn't hit anything before hiting the point, then color it
        	if (! Shader::hit_anything( Ray(ht.p, world->lum[i]->direction), 0.001f, std::numeric_limits<float>::infinity(), ht_s) )
        	{
	            // Light direction normalized
	            auto l = unit_vector(world->lum[i]->direction - r_.get_direction());

	            // Difuse component
	            auto difuse = ht.mat->albedo * std::max(0.f, dot(l, ht.normal)) * world->lum[i]->intensity;
	            
	            auto h = unit_vector(l + v); 

	            // Specular component
	            auto specular = ht.mat->ks * pow ( std::max(0.f, dot(ht.normal, h)) , ht.mat->alpha ) * world->lum[i]->intensity;

	            if (depth_ > 0)
	            {
	            	ip += ht.mat->km*color( Ray(ht.p , reflected),  0.001f, std::numeric_limits<float>::infinity(), depth_ - 1);
	            }
	            ip += difuse + specular;
	        }
        }

        // Remove any misscalculation to the [0;1] range
        if (ip.r() > 1){
            ip[0] = 1;
        }
        if (ip.g() > 1){
            ip[1] = 1;
        }
        if (ip.b() > 1){
            ip[2] = 1;
        }
        return ip;
    }

    // Else, dye the pixel with the background color

    rgb result = Shader::interpolation_biline(r_);
    return result;
}

#endif
