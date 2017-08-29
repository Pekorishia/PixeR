#ifndef _SHADER_H_
#define _SHADER_H_

#include "scene.h"

class Shader
{
public:

	Scene *wold;

	Shader(Scene *s_){
		wold = s;
	}
	
	virtual rgb color();//falta as coisas;
	
};

#endif