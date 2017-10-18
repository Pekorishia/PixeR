#ifndef _SpotLight_H_
#define _SpotLight_H_

#include "light.h" 

#include "../utility/vec3.h"

class SpotLight : public Light
{
public:

	point3 origin;
	float angle;

	SpotLight( point3 origin_, vec3 direction_, rgb intensity_, float angle_)
	{
		Light::intensity = intensity_;
		Light::direction = direction_;

		origin = origin_;
		angle = angle_;
	}

	virtual vec3 getDirection(const point3 & p_) const;
	virtual vec3 getIntensity() const;

};

vec3 SpotLight::getDirection(const point3 & p_) const
{
	vec3 dir = origin - p_;
	vec3 direcao = origin - Light::direction;

	// calculate the cosine between the direction of the light and the position of the light
	// cos θ = (u * v) / (||u|| * ||v||)
    float cosine = dot(direcao, dir) / ((direcao).length() * dir.length());
    
    if (cosine >= angle) { return dir; }

	return vec3(0, 0, 0);
}

vec3 SpotLight::getIntensity() const
{
       return Light::intensity;
}

#endif