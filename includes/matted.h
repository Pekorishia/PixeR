#ifndef _MATTED_H_
#define _MATTED_H_

#include "object.h"
#include "material.h" 

class Matted: public Material 
{
	public:

		rgb albedo; //Kd o difuso.
		rgb ks;
		rgb ka;
		float alpha;

		Matted(const rgb albedo_
			//, const rgb ks_, const rgb ka_, float alpha_
			)
		{
			albedo = albedo_;
			//ks = ks_;
			//ka = ka_;
			//alpha = alpha_;
		}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;

	protected:

        vec3 random_in_unit_sphere() const;
	
};

vec3 Matted::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);

    return p;
}

bool Matted::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
	vec3 p_ = random_in_unit_sphere();
    vec3 target = ht_.p + ht_.normal + p_;
    scattered_ray = Ray(ht_.p, target - ht_.p);
    attenuation_ = albedo;

    return true;
}

#endif