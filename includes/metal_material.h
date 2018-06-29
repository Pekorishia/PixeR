#ifndef _METAL_H_
#define _METAL_H_

#include "object.h"
#include "texture.h"
#include "material.h"

class Metal : public Material
{
public:

    float fuzziness;

	Metal( Texture *albedo_, float fuzz){
		Material::albedo = albedo_;
        fuzziness = fuzz;
	}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2 ) const;
    virtual vec3 emitted(float u, float v, const vec3& p) const{
        return vec3 (0,0,0);
    }

protected:

    vec3 reflect(const vec3 v, const vec3 n) const;	
    
    vec3 random_in_unit_sphere() const;
    
};



vec3 Metal::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);

    return p;
}



vec3 Metal::reflect(const vec3 v, const vec3 n) const
{
    return v - 2*dot(v, n)*n;
}

bool Metal::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2 ) const
{
    vec3 reflected = reflect(unit_vector(r_.get_direction()), ht_.normal);
    scattered_ray = Ray(ht_.p, reflected + fuzziness*random_in_unit_sphere());
    attenuation_ = Material::albedo->value(0,0, vec3(0,0,0));

    return (dot(scattered_ray.get_direction(), ht_.normal) > 0);
}

#endif