#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "object.h"

#include "../utility/ray.h"
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
	rgb ks;
	rgb ka;
	float alpha;

	
	//~Material();
	virtual bool scartter (const Ray & r_, HitRecord & ht_, vec3 & attenuation_, Ray & scattered_ray) const = 0;
	
};

#endif