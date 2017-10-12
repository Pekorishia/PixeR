#ifndef _DIRECTIONLIGHT_H_
#define _DIRECTIONLIGHT_H_

#include <math.h>  

#include "light.h" 

class Direction_light : public Light
{
public:

	Direction_light( vec3 direction_, rgb intensity_)
	{
		Light::direction = direction_;
		Light::intensity = intensity_;
	}

	virtual vec3 get_direction(const point3 & p_) const;

};

vec3 Direction_light::get_direction(const point3 & p_) const
{
	return Light::direction;
}

#endif