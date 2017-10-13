#ifndef _LIGTH_H_
#define _LIGTH_H_

#include "../utility/vec3.h"

class Light
{
public:
	vec3 direction;
	rgb intensity;

	virtual vec3 getDirection(const point3 & p_) const = 0;	
	virtual vec3 getIntensity(const point3 & p_) const = 0;
	
};

#endif
