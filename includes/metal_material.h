#ifndef _METAL_H_
#define _METAL_H_

#include "object.h"
#include "material.h"

class Metal : public Material
{
public:
	Metal(const rgb albedo_){
		Material::albedo = albedo_;
	}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;

protected:

    vec3 reflect(const vec3 v, const vec3 n) const;	
};


vec3 Metal::reflect(const vec3 v, const vec3 n) const
{
    return v - 2*dot(v, n)*n;
}

bool Metal::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
    vec3 reflected = reflect(unit_vector(r_.get_direction()), ht_.normal);
    scattered_ray = Ray(ht_.p, reflected);
    attenuation_ = Material::albedo;

    return (dot(scattered_ray.get_direction(), ht_.normal) > 0);
}

#endif