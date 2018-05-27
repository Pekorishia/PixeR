#ifndef _BlinnPhong_H_
#define _BlinnPhong_H_

#include "object.h"
#include "texture.h"
#include "material.h"
#include "shader.h" 

class BlinnPhong: public Material 
{
	public:

		BlinnPhong(Texture *albedo_, const rgb ks_,  const rgb km_, const rgb ka_, float alpha_)
		{
			Material::albedo = albedo_;
			Material::ks = ks_;
			Material::km = km_;
			Material::ka = ka_;
			Material::alpha = alpha_;
		}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
	virtual vec3 emitted(float u, float v, const vec3& p) const{
		return vec3 (0,0,0);
	}
	
};


bool BlinnPhong::scatter (const Ray & r_, const HitRecord & ht, vec3 & ip, Ray & scattered_ray) const
{
    return true;
}

#endif