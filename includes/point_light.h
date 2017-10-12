#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include "light.h" 

#include "../utility/vec3.h"

class Point_light : public Light
{
public:

	point3 position;

	Point_light( point3 position_, rgb intensity_)
	{
		position = position_;
		Light::intensity = intensity_;
	}

	virtual vec3 get_direction(const point3 & p_) const;

};

vec3 Point_light::get_direction(const point3 & p_) const
{
	//if(position.length() > p_.length())
		return position - p_;


	
}

#endif