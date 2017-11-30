#ifndef _Checker_texture_H_
#define _Checker_texture_H_

#include "scene.h"
#include "texture.h"
#include <math.h> 

class Checker_texture : public Texture
{
public:

	Texture *odd;
	Texture *even;

	Checker_texture();
	Checker_texture( Texture *t0, Texture *t1)
	{
		even = t0;
		odd = t1;
	}


	virtual rgb value(float u = 0, float v = 0, const vec3& p = vec3()) const
	{
		float sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
		if(sines < 0){
			return odd->value(u, v, p);
		}else
		{
			return even->value(u, v, p);
		}
	}
	
};

#endif