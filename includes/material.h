#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "../utility/vec3.h"

class Material
{
public:
	//definir o P ou alfa que Será o tamanho da bola de luz.
	//luz ambiente, Ka
	/*KS. especular é o brilho. 
	A possição do olho define a intessidade. 
	Depende da posição da camera, da luz e da normal.*/

	rgb albedo; //Kd o difuso.

	Material(rgb albedo_)
	{
		albedo = albedo_;
	}
	//~Material();
	virtual bool scartter (const ray & r_, hitRecord & ht_, vec3 & v_, Ray &) = 1;
	
};

#endif
