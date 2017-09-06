#ifndef _LIGTH_H_
#define _LIGTH_H_

#include "../utility/vec3.h"

class Light
{
public:
	vec3 direction;
	rgb intensity;

	Light( vec3 direction_, rgb intensity_)
	{
		direction = direction_;
		intensity = intensity_;
	}
	//~Ligth();
	
};

#endif
