#ifndef _Diffuse_light_H_
#define _Diffuse_light_H_

#include "object.h"
#include "texture.h"
#include "material.h"

class Diffuse_light : public Material
{
public:

    Texture *emit;

	Diffuse_light(Texture *emit_){
		emit = emit_;
	}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
    
    virtual vec3 emitted(float u, float v, const vec3& p) const;
    
};



vec3 Diffuse_light::emitted(float u, float v, const vec3& p) const
{
    return emit->value(u,v,p);
}



bool Diffuse_light::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const
{
     return false;
}

#endif