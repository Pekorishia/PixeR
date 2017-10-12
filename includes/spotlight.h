#ifndef _SPOTLIGTH_H_
#define _SPOTLIGTH_H_

#include "../utility/vec3.h"
#include "Light.h"

class SpotLight : public Light
{
public:
	vec3 direction;
	rgb intensity;
	point3 origin;
	float angle;

	Light( vec3 direction_, rgb intensity_, point3 origin)
	{
		Light::direction = direction_;
		Light::intensity = intensity_;
	}
	//~Ligth();
	
};

#endif
