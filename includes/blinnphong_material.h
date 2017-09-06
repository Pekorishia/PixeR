#ifndef _BlinnPhong_H_
#define _BlinnPhong_H_

#include "object.h"
#include "material.h" 

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
	
};


bool BlinnPhong::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
    return true;
}

#endif