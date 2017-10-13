#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "light.h" 

#include "../utility/vec3.h"

class PointLight : public Light
{
public:

	point3 origin;

	PointLight( point3 origin_, rgb intensity_)
	{
		origin = origin_;
		Light::intensity = intensity_;
	}

	virtual vec3 getDirection(const point3 & p_) const;
	virtual vec3 getIntensity(const point3 & p_) const;

};

vec3 PointLight::getDirection(const point3 & p_) const
{
		return origin - p_;
}

vec3 PointLight::getIntensity(const point3 & p_) const
{
		return Light::intensity;
}

#endif