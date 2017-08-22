#include <iostream>
#include "../includes/sphere.h"
//#include "../includes/object.h"
#include "../includes/scene.h"

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

rgb color( const Ray & r_ , Scene *world)
{
	HitRecord ht;

	if ( world->found( r_, 0.0, 4.0, ht) ) {
		return 0.5 * rgb(ht.normal.x() + 1, ht.normal.y() + 1, ht.normal.z() + 1);
	}

	return vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.7, 1 ));
}

int main (){

    int n_cols{ 200 };     
    int n_rows{ 100 };

    int n_samples{ 100 }; 

	std::cout << "P3\n" << n_cols << " " << n_rows << "\n255\n";

    //=== Defining our 'camera'
    point3 lower_left_corner(-2, -1, -1); // lower left corner of the view plane.
    vec3 horizontal(4, 0, 0); // Horizontal dimension of the view plane.
    vec3 vertical(0, 2, 0); // Vertical dimension of the view plane.
    point3 origin(0, 0, 0); // the camera's origin.

    Object *list[3];

    list[0] = new Sphere(point3 (0,0,-1), 0.5);
    list[1] = new Sphere(point3 (0,-100.5,-1), 100);
    list[2] = new Sphere(point3 (0,-100.5,-1), 100);

    Scene *world  = new Scene(list, 3);

     // NOTICE: We loop rows from bottom to top.
    for ( auto row{n_rows-1} ; row >= 0 ; --row ) // Y
    {
        for( auto col{0} ; col < n_cols ; col++ ) // X
        {
            // Determine how much we have 'walked' on the image: in [0,1]
            auto u = float(col) / float( n_cols ); // walked u% of the horizontal dimension of the view plane.
            auto v = float(row) / float( n_rows ); // walked v% of the vertical dimension of the view plane.

            // Determine the ray's direction, based on the pixel coordinate (col,row).
            // We are mapping (matching) the view plane (vp) to the image.
            // To create a ray we need: (a) an origin, and (b) an end point.
            //
            // (a) The ray's origin is the origin of the camera frame (which is the same as the world's frame).
            //
            // (b) To get the end point of ray we just have to 'walk' from the
            // vp's origin + horizontal displacement (proportional to 'col') +
            // vertical displacement (proportional to 'row').
            point3 end_point = lower_left_corner + u*horizontal + v*vertical ;
            // The ray:
            Ray r( origin, end_point - origin );

            // Determine the color of the ray, as it travels through the virtual space.
            auto c = color( r, world );

            int ir = int( 255.99f * c[rgb::R] );
            int ig = int( 255.99f * c[rgb::G] );
            int ib = int( 255.99f * c[rgb::B] );

            std::cout << ir << " " << ig << " " << ib << "\n";
                     
        }
    }

	return 0;
}