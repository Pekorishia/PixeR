#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_
#include "material.h"
#include "object.h"

class Lambertian : public Material{
	public:
		vec3 albedo;
		
		Lambertian(const vec3 &attenuation){
			albedo = attenuation;
		}

		vec3 random_in_unit_sphere() const{
			vec3 p;
			do{
				p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
			}while(dot(p,p) >= 1.0);
			return p;
		}
			
		virtual bool scatter(const Ray &r, const HitRecord &ht, vec3 & attenuation, Ray &scattered) const {
			vec3 target = ht.p + ht.normal + random_in_unit_sphere();
			scattered = Ray(ht.p, target-ht.p);
			attenuation = albedo;
			return true;
		}
};

#endif
