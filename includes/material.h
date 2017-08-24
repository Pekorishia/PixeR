#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "../utility/vec3.h"

class Material
{
public:

	rgb albedo;

	Material(rgb albedo_)
	{
		albedo = albedo_;
	}
	//~Material();
	
};

#endif
