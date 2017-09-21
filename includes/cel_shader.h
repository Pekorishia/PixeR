#ifndef _CELSHADER_H_
#define _CELSHADER_H_

#include "shader.h" 

class CelShader : public Shader
{
	public:
		CelShader(Scene *world_){
			 Shader::world = world_;
		}

		 virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
	
};


rgb CelShader::color(const Ray & r_, float t_min, float t_max, int depth_) const{
	return (0,0,0);
}

#endif