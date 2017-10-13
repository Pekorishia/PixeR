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
	virtual vec3 getIntensity(const point3 & p_) const;

};

vec3 SpotLight::getDirection(const point3 & p_) const
{
	vec3 dir = origin - p_;
    float actual_angle = dot(-Light::direction, dir) / ((-Light::direction).length() * dir.length());
    
    if (actual_angle >= angle) { return dir; }

	return vec3(0, 0, 0);
}

vec3 SpotLight::getIntensity(const point3 & p_) const
{
       return Light::intensity;
}

#endif