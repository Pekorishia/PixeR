#ifndef _Lambertian_H_
#define _Lambertian_H_

#include "object.h"
#include "material.h" 

class Lambertian: public Material 
{
	public:

		Lambertian(const rgb albedo_)
		{
			Material::albedo = albedo_;
			Material::ks = rgb(0,0,0);
			Material::ka = rgb(0,0,0);
			Material::alpha = 0;
		}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;

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

bool Lambertian::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
	vec3 p_ = random_in_unit_sphere();
    vec3 target = ht_.p + ht_.normal + p_;
    scattered_ray = Ray(ht_.p, target - ht_.p);
    attenuation_ = Material::albedo;

    return true;
}

#endif