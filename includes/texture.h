#ifndef _Texture_H_
#define _Texture_H_

#include "scene.h"
#include <math.h> 

#include "../utility/ray.h"

class Texture
{
public:

	virtual rgb value(float u, float v, const vec3& p) const = 0;

};


#endif