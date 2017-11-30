#ifndef _CONSTANT_TEXTURE_H_
#define _CONSTANT_TEXTURE_H_

#include "scene.h"
#include <math.h> 

class Constant_texture : public Texture
{
public:

	rgb color;

	Constant_texture();
	Constant_texture( rgb color_)
	{
		color = color_;
	}


	virtual rgb value(float u = 0, float v = 0, const vec3& p = vec3()) const
	{
		return color;
	}
	
};

#endif