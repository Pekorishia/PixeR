#ifndef _CEL_H_
#define _CEL_H_

#include "object.h"
#include "material.h"
#include "shader.h" 

class Cel : public Material
{
	public:
		Cel(){

		}
		virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
};

bool Cel::scatter (const Ray & r_, const HitRecord & ht, vec3 & ip, Ray & scattered_ray) const
{
    return true;
}


#endif