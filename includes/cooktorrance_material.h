#ifndef _CookTorranceMaterial_H_
#define _CookTorranceMaterial_H_

#include "object.h"
#include "texture.h"
#include "material.h"

//#define PI 3.141592

class CookTorranceMaterial : public Material
{
public:

	CookTorranceMaterial(Texture *albedo_, float m_){
		Material::albedo = albedo_;
        Material::m = m_;
	}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
    virtual vec3 emitted(float u, float v, const vec3& p) const{
        return vec3 (0,0,0);
    }
    
};


bool CookTorranceMaterial::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
    return true;
}

#endif