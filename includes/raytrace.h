#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <fstream>

#include "camera.h"
#include "scene.h"

//#include "../utility/vec3.h"
#include "../utility/ray.h"

class Raytrace
{
	public:

		Camera *cam;
		Scene *world;
        Shader *shade;
		int n_rows;
		int n_cols;
		int n_samples;
		int ray_depth;

        float t_min;
        float t_max;

		Raytrace(Camera *cam_, Scene *s_, Shader *shade_, int col_ = 1200, int row_ = 600, int samples_ = 100, int depth_ = 10, float t_min_ = 0.001f, float t_max_ = std::numeric_limits<float>::infinity())
		{
			cam = cam_;
			world = s_;
            shade = shade_;
			n_rows = row_;
			n_cols = col_;
			n_samples = samples_;
			ray_depth = depth_;
            t_min = t_min_;
            t_max = t_max_;
		}

	    void render();

};


void Raytrace::render () 
{ 	
   	std::cout << "P3\n" << n_cols << " " << n_rows << "\n255\n";

     // NOTICE: We loop rows from bottom to top.
    for ( auto row{n_rows-1} ; row >= 0 ; --row ) // Y
    {
        for( auto col{0} ; col < n_cols ; col++ ) // X
        {
            rgb hue(0,0,0);

            for (int s = 0; s < n_samples; ++s)
            {
                // Determine how much we have 'walked' on the image: in [0,1]
                auto u = float(col + drand48()) / float( n_cols ); // walked u% of the horizontal dimension of the view plane.
                auto v = float(row + drand48()) / float( n_rows ); // walked v% of the vertical dimension of the view plane.
              
                Ray r = cam->get_ray(u,v);

                hue += shade->color( r, t_min, t_max );
            }
            
            hue /= float(n_samples);
            hue = vec3(sqrt(hue[rgb::R]), sqrt(hue[rgb::G]), sqrt(hue[rgb::B]) );

            int ir = int( 255.99f * hue[rgb::R] );
            int ig = int( 255.99f * hue[rgb::G] );
            int ib = int( 255.99f * hue[rgb::B] );

            std::cout << ir << " " << ig << " " << ib << "\n";                     
        }
    }
}

#endif