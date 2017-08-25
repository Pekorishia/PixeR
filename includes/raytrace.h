#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <fstream>

#include "camera.h"
#include "scene.h"
#include "material.h"

//#include "../utility/vec3.h"
#include "../utility/ray.h"

class Raytrace
{
	public:

		Camera *cam;
		Scene *world;
		//Material materials;
		int n_rows;
		int n_cols;
		int n_samples;
		int ray_depth;

		Raytrace(Camera *cam_, Scene *s_, int col_ = 1200, int row_ = 600, int samples_ = 100, int depth_ = 10)
		{
			cam = cam_;
			world = s_;
			n_rows = row_;
			n_cols = col_;
			n_samples = samples_;
			ray_depth = depth_;
		}

		// + render() = ray_trace()

		rgb color(const Ray & r_ , Scene *world);
	    bool hit_anything( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ );
	    void render();

    private:

    	rgb vertical_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_);
	
};

rgb Raytrace::color( const Ray & r_ , Scene *world)
{
	HitRecord ht;

	if ( hit_anything( r_, 0.0, 4.0, ht) ) {
		return 0.5 * rgb(ht.normal.x() + 1, ht.normal.y() + 1, ht.normal.z() + 1);
	}

	return vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
}


bool Raytrace::hit_anything( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ )
{
    HitRecord temp_ht;

    bool hit_anything = false;
    double closest_so_far = t_max_;

    for( int i = 0; i < world->list_size; i++){
        if (world->list[i]->hit(r_, t_min_, closest_so_far, temp_ht)){
            hit_anything = true;
            closest_so_far = temp_ht.t;
            ht_ = temp_ht;
        }
    }

    return hit_anything;
}

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

                hue += color( r, world );
            }
            
            hue /= float(n_samples);

            int ir = int( 255.99f * hue[rgb::R] );
            int ig = int( 255.99f * hue[rgb::G] );
            int ib = int( 255.99f * hue[rgb::B] );

            std::cout << ir << " " << ig << " " << ib << "\n";                     
        }
    }
}

rgb Raytrace::vertical_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_)
{    
    // Make the ray a vector in the same direction.     
    auto ray = r_.get_direction();     

    // Take only the vertical component, since the lerp has to interpolate colors verticaly     
    auto ray_y = ray.y(); // this component might assume values ranging from -1 to 1     

    // Normalize the ray's vertical component to the range [0;1]     
    auto t= 0.5+( ray_y*0.5 );     

    // Use linear interpolation (lerp) between the colors that compose the background     
    rgb result = bottom_*(1 - t) + top_*(t);     
    return result; 
}

#endif