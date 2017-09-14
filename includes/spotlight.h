#ifndef _SPOTLIGTH_H_
#define _SPOTLIGTH_H_

#include "../utility/vec3.h"

class SpotLight
{
public:
	vec3 direction;
	rgb intensity;
	point3 origin;
	float angle;

	Light( vec3 direction_, rgb intensity_)
	{
		direction = direction_;
		intensity = intensity_;
	}
	//~Ligth();
	
};

#endif
