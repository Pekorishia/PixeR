#include <iostream>
#include <fstream>

#include "../includes/scene.h"
#include "../includes/sphere.h"
#include "../includes/raytrace.h"



/*  
 * Find the resulting color of the vertical interpolation  
 *  
 * TODO: only work with the range [-1;1]  
 
rgb vertical_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_)
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

/*  
 * Find the resulting color of the horizontal interpolation  
 *  
 * TODO: only work with the range [-2;2]  
 */ 
rgb horizontal_interpolation ( const Ray & r_, const rgb & left_, const rgb & right_)
{     
    // Make the ray a vector in the same direction.     
    auto ray = r_.get_direction();     

    // Take only the horizontal component     
    auto ray_x = ray.x(); // this component might assume values ranging from -2 to 2     

    // Normalize the ray's horizontal component to the range [0;1]     
    auto t= 0.5+( ray_x*0.25 );     

    // Use linear interpolation (lerp) between the colors that compose the background     
    rgb result = left_*(1 - t) + right_*(t);     

    return result; 
} 

/*  
 * Find the resulting color of the depth interpolation 
 */ 
rgb depth_interpolation ( float t_, float min_depth, float max_depth, const rgb & foreground_, const rgb & background_)
{    
    float t_normalized;

    if (t_ <= max_depth && t_ >= min_depth){
        t_normalized = t_ / max_depth;     
    }
    else{
        t_normalized = 1;
    }

    // Use linear interpolation (lerp) between the colors that compose the background     
    rgb result = foreground_*(1 - t_normalized) + background_*(t_normalized);     
    return result; 
}

/*
 * Returns the color based on the normal vector
 */
rgb normal_color (const Ray & r_, const point3 & c_, float t_)
{   

    point3 p = r_.point_at(t_);

    vec3 normal = p - c_;

    auto unit_normal = utility::unit_vector(normal);

    // Nomalizing the unit normal to only positive values
    auto color = 0.5 * (unit_normal + vec3(1,1,1));
        
    return color;
}

/*
rgb color( const Ray & r_ , Scene *world)
{
	HitRecord ht;

	if ( world->found( r_, 0.0, 4.0, ht) ) {
		return 0.5 * rgb(ht.normal.x() + 1, ht.normal.y() + 1, ht.normal.z() + 1);
	}

	return vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
}*/

int main (){

    int n_cols{ 1000 };     
    int n_rows{ 500 };
    int n_samples{ 100 }; 
    int ray_depth{ 10 };

    Object *list[2];

    list[0] = new Sphere(point3 (0, 0, -1), 0.5);
    //list[1] = new Sphere(point3 (0, 1, -2), 0.6);
    //list[2] = new Sphere(point3 (-0.4, 0, -3), 0.7);
    list[1] = new Sphere(point3 (0, -100.5, -1), 100);

    Scene *world  = new Scene(list, 2);

    Camera *cam = new Camera();

    Raytrace *raytrace = new Raytrace (cam, world, n_cols, n_rows,n_samples, ray_depth);

    raytrace->render();

	return 0;
}