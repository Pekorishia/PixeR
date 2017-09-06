#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "object.h"
#include "../utility/ray.h"
#include "../utility/vec3.h"

class Material{
	public:
	//definir o P ou alfa que Será o tamanho da bola de luz.
	//luz ambiente, Ka
	/*KS. especular é o brilho. 
	A posição do olho define a intessidade. 
	Depende da posição da camera, da luz e da normal.*/

		rgb albedo; //Kd o difuso.
		rgb ks;
		rgb ka;
		float alpha;
		virtual bool scatter(const Ray &r, const struct HitRecord &ht, vec3 & attenuation, Ray &scatterd) const = 0;
};

#endif