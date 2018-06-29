#ifndef _Lambertian_H_
#define _Lambertian_H_

#include "object.h"
#include "material.h" 
#include "texture.h"
#include "constant_texture.h"

class Lambertian: public Material 
{
	public:

		Lambertian(Texture *albedo_)
		{
			Material::albedo = albedo_;
			Material::ks = rgb(0,0,0);
			Material::ka = rgb(0,0,0);
			Material::alpha = 0;
		}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2 ) const;
	
	virtual vec3 emitted(float u, float v, const vec3& p) const{
		 	return vec3(0,0,0);
	}

	protected:

        vec3 random_in_unit_sphere() const;
	
};

vec3 Lambertian::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);

    return p;
}

bool Lambertian::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2 ) const
{
	vec3 p_ = random_in_unit_sphere();
    vec3 target = ht_.p + ht_.normal + p_;
    scattered_ray = Ray(ht_.p, target - ht_.p);
    attenuation_ = Material::albedo->value(0,0, ht_.p);

    return true;
}

#endif