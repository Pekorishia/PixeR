#ifndef _LIGTH_H_
#define _LIGTH_H_

#include "../utility/vec3.h"

class Light
{
public:
	vec3 direction;
	rgb intensity;

	virtual vec3 get_direction(const point3 & p_) const = 0;
	
	
};

#endif
