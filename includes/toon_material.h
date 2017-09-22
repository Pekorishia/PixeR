#ifndef _Toon_H_
#define _Toon_H_

#include "object.h"
#include "material.h"
#include "shader.h"
#include <vector>

class Toon : public Material
{
	public:

		Toon(std::vector<rgb> gradient_){
			Material::gradient = gradient_;
			//std::cout << *Material::gradient[0];
			//Material::angles = angles_;

		}
		virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const;
};

bool Toon::scatter (const Ray & r_, const HitRecord & ht, vec3 & ip, Ray & scattered_ray) const
{
    return true;
}


#endif