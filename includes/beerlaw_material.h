#ifndef _BeerLaw_H_
#define _BeerLaw_H_

#include "object.h"
#include "texture.h"
#include "material.h"

class BeerLaw : public Material
{
public:

	BeerLaw(Texture *albedo_, float ri){
		Material::albedo = albedo_;
        Material::ref_idx = ri;
	}

	virtual bool scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2 ) const;
    virtual vec3 emitted(float u, float v, const vec3& p) const{
        return vec3 (0,0,0);
    }

protected:

    vec3 reflect(const vec3 v, const vec3 n) const;	
    
    vec3 random_in_unit_sphere() const;

    bool refract(const vec3 & v, const vec3& n, float ni_over_nt, vec3& refracted) const;

    float schlick(float cosine, float ref_idx) const;
    
};



vec3 BeerLaw::random_in_unit_sphere() const
{
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }while(dot(p,p) >= 1.0);

    return p;
}



vec3 BeerLaw::reflect(const vec3 v, const vec3 n) const
{
    float cosI = - dot(n, v);
    return v + 2*cosI*n;
}


bool BeerLaw::refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant =  1.f - (ni_over_nt*ni_over_nt * (1.f -(dt*dt)));
    if(discriminant > 0.f){
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }else
        return false;
}


float BeerLaw::schlick(float cosine, float ref_idx) const {
    float r = (1.f - ref_idx) / (1.f + ref_idx);
    r = r * r;
    return (r + (1.f - r) * std::pow((1.f - cosine), 5.f));
}



bool BeerLaw::scatter (const Ray & r_, const HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray, float &reflect_prob , Ray &scatterd2  ) const
{
    vec3 outward_normal, t, n, d ;
    float cosine;

    n = unit_vector(ht_.normal);
    d = unit_vector(r_.get_direction()); 
    vec3 r = reflect(d, n);
    attenuation_ = Material::albedo->value(0,0,vec3(0,0,0));

    if( dot(d,n) < 0){
        refract(d, n, ref_idx, t);
        cosine = dot(-d,n);
        attenuation_ = rgb(1,1,1);
    }else{
        rgb absorb;
        float distan = ht_.t;
        absorb[0] =  std::min(1.f, std::max(0.f, std::exp(distan * -1 * attenuation_.x())));
        absorb[1] =  std::min(1.f, std::max(0.f, std::exp(distan * -1 * attenuation_.y())));
        absorb[2] =  std::min(1.f, std::max(0.f, std::exp(distan * -1 * attenuation_.z())));
        attenuation_ = absorb;
        if( refract(d, -n, 1/ref_idx, t))
            cosine = dot(t, n);
        else{
            scattered_ray = Ray(ht_.p, r);
            return false;
        }
    }
    reflect_prob = schlick(cosine, ref_idx);
    scattered_ray = Ray(ht_.p, r);
    scatterd2 = Ray(ht_.p, t);

    return true;
        
}

#endif