#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include "camera.h"
#include "scene.h"
#include "material.h"

#include "../utility/vec3.h"

// class RayTrace
//     + Camera c
//     + Scene s
//     + ResourceManager materials
//     + n_cols
//     + n_rows
//     + n_samples
//     + ray_depth
//     ----------------
//     + init()
//     + hit_anything()
//     + render() = ray_trace()
//     + color()


class Raytrace
{
public:

	Camera cam;
	Scene s;
	Material materials
	int n_rows;
	int n_cols;
	int n_samples;
	int ray_depth;


	Raytrace();
	//~Raytrace();

	rgb color(const Ray & r_ , Scene *world);
	
};

rgb Raytrace::color( const Ray & r_ , Scene *world)
{
	HitRecord ht;

	if ( world->found( r_, 0.0, 4.0, ht) ) {
		return 0.5 * rgb(ht.normal.x() + 1, ht.normal.y() + 1, ht.normal.z() + 1);
	}

	return vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
}

#endif