#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <math.h> 
#include <cmath>     

#include "../utility/vec3.h"
#include "../utility/ray.h"

using namespace utility;

/*
 *  In the near future we'll want to refactor our project to become
 *  something like the code below.
 */
#ifdef TRADITIONAL_RT
Image Raytrace (Camera cam, Scene scene, int width, int height)
{
	Image image = new Image (width, height) ;
	for (int i = 0 ; i < height ; i++)
		for (int j = 0 ; j < width ; j++) {
			Ray ray = RayThruPixel (cam, i, j) ;
			Intersection hit = Intersect (ray, scene) ;
			image[i][j] = FindColor (hit) ;
			}
	return image ;
}
#endif

/*
 * Returns de Z component, if the ray hits the sphere, or infinity otherwise
 * 
 * TODO: only works with a 2D sphere
 */
float hit_sphere ( const Ray & r_, const point3 & c_, float radius_ ){

	// First, manipulate the 1), 2) and 3) line to create the 4) equation
	// 1) (point - center) * (point - center) - radius² = 0
	// 2) point == ray
	// 3) ray(t) = origin + direction*t
	// 4) d*d*t² + 2(origin - center)*d*t + (origin - center)*(origin - center) - radius² = 0


	// Second, calculate the a, b and c from the equation 4) (a*t² + b*t + c = 0) 
	auto oc = r_.get_origin() - c_; // (origin - center)

	auto a = utility::dot( r_.get_direction(), r_.get_direction() );

	auto b = 2 * utility::dot( oc, r_.get_direction() );

	auto c = utility::dot(oc, oc) - (radius_ * radius_);

	// Third, calculate the delta (b² - 4ac) is >= 0
	auto delta = b * b - 4 * a * c;

	// Last, return the Z component
	if (delta >= 0){
		return std::abs (c_.z());
	}

	return std::numeric_limits<float>::infinity();
}

//float euclidian_distance (const Ray & r_, const point3 & c_){
//    float distance = sqrt(pow (r_.x() - c_.x(), 2) + pow (r_.y() - c_.y(),2));
//    return distance;
//}

//rgb radial_gradient (const Ray & r_, const point3 & c_, float radius_){
//	float distance = euclidian_distance(r_, c_);
//	int val = 255.0/radius_;

    //return the gradient
//    return rgb (255 - (((int)val * (int)distance) % 255), 0, 0);
//}

/*
 * Find the resulting color of the horizontal interpolation
 * 
 * TODO: only work with the range [-2;2]
 */
rgb horizontal_interpolation ( const Ray & r_, const rgb & left, const rgb & right){
	// 1) we make the ray a vector in the same direction.
    auto ray = r_.get_direction();

    // 2) we take only the horizontal component
    auto ray_x = ray.x(); // this component might assume values ranging from -2 to 2

    // 3) normalize the ray's horizontal component to the range [0;1]
    auto t= 0.5+( ray_x*0.25 );

    // 4) use linear interpolation (lerp) between the colors that compose the background
    rgb result = left*(1 - t) + right*(t);

    return result;
}

/*
 * Find the resulting color of the vertical interpolation
 *
 * TODO: only work with the range [-1;1]
 */
rgb vertical_interpolation ( const Ray & r_, const rgb & bottom, const rgb & top){
	// 1) we make the ray a vector in the same direction.
    auto ray = r_.get_direction();

    // 2) we take only the vertical component, since the lerp has to interpolate colors verticaly
    auto ray_y = ray.y(); // this component might assume values ranging from -1 to 1

    // 3) normalize the ray's vertical component to the range [0;1]
    auto t= 0.5+( ray_y*0.5 );

    // 4) use linear interpolation (lerp) between the colors that compose the background
    rgb result = bottom*(1 - t) + top*(t);

    return result; 
}


rgb color( const Ray & r_ ){
	rgb top_left     (0, 1, 0); // green
	rgb top_right    (1, 1, 0); // yellow
	rgb bottom_left  (0, 0, 0); // black
	rgb bottom_right (1, 0, 0); // red

    
    //float hit1 = hit_sphere (r_, point3(0,0,-1), 0.2);
	
    //if ( hit1 < std::numeric_limits<float>::infinity()){
    //	return radial_gradient (r_, point3(0,0,-1), 0.2);
    //}
    

    rgb top = horizontal_interpolation(r_, top_left, top_right);
    rgb bottom = horizontal_interpolation(r_, bottom_left, bottom_right);

    rgb result = vertical_interpolation(r_, bottom, top);
    
    return result;
}

/*
 * Opens the scene file and get all values
 */
//void file_handler( std::string & file_name, 
//				   std::string & img_name, 
//				   std::string & type,
//				   std::string & codification,
//				   int & width,
//				   int & height,
//				   rgb & top_left,
//				   rgb & bottom_left,
//				   rgb & top_right,
//				   rgb & bottom_right){
//
//	std::string line;
//  	std::ifstream ifs(file_name);
//
//  	getline (ifs,line);
//
//  	std::cout << line << ".\n";
//
//    // Remove comments
//    if (line.find("#") < line.length()) {
//        line = line.replace(line.find("#"), line.length() - 1, "");
//    }
//
//    std::cout << line << std::endl;
//    std::string n("    UPPER_LEFT: ");
//    line = line.replace(0, n.length(), "");
//    std::cout << line << std::endl;
//    n = line.substr(0, line.find(" "));
//    float r = atof(n.c_str());
//    line = line.replace(1, line.find(" "), "");
//    std::cout << line << std::endl;
//    n = line.substr(0, line.find(" "));
//    std::cout << n << std::endl;
//    float g = atof(n.c_str());
//    n = line.substr(line.find(" "));
//    std::cout << n << std::endl;
//    float b = atof(n.c_str());
//    std::cout << r << " - " << g << " - " << b << std::endl;
//}

int main (int argc, char **argv){
	
	//file_handler(argv[1]);	

    int n_cols{ 200 };
    int n_rows{ 100 };

    std::cout << "P3\n"
              << n_cols << " " << n_rows << "\n"
              << "255\n";

    //=== Defining our 'camera'
    point3 lower_left_corner( -2, -1, -1 ); // lower left corner of the view plane.
    vec3 horizontal( 4, 0, 0 ); // Horizontal dimension of the view plane.
    vec3 vertical(0, 2, 0); // Vertical dimension of the view plane.
    point3 origin(0, 0, 0); // the camera's origin.

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
            auto c = color( r );

            int ir = int( 255.99f * c[rgb::R] );
            int ig = int( 255.99f * c[rgb::G] );
            int ib = int( 255.99f * c[rgb::B] );

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
