#ifndef _DIRECTIONLIGHT_H_
#define _DIRECTIONLIGHT_H_

#include "light.h" 

#include "../utility/vec3.h"

class DirectionLight : public Light
{
public:

	DirectionLight( vec3 direction_, rgb intensity_)
	{
		Light::intensity = intensity_;
		Light::direction = direction_;
	}

	virtual vec3 getDirection(const point3 & p_) const;
	virtual vec3 getIntensity() const;

};

vec3 DirectionLight::getDirection(const point3 & p_) const
{
		return Light::direction;
}
vec3 DirectionLight::getIntensity() const
{
		return Light::intensity;
}

#endif