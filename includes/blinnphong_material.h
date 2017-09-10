#ifndef _BlinnPhong_H_
#define _BlinnPhong_H_

#include "object.h"
#include "material.h"
#include "shader.h" 

class BlinnPhong: public Material 
{
	public:

		BlinnPhong(const rgb albedo_, const rgb ks_, const rgb ka_, float alpha_)
		{
			Material::albedo = albedo_;
			Material::ks = ks_;
			Material::ka = ka_;
			Material::alpha = alpha_;
		}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
	
	protected:

        vec3 random_in_unit_sphere() const;
	
};

vec3 BlinnPhong::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);

    return p;
}


bool BlinnPhong::scatter (const Ray & r_, const HitRecord & ht, vec3 & ip, Ray & scattered_ray) const
{
	    

    return true;
}

#endif