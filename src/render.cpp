#include <iostream>
#include <sstream>
#include <string> 
#include <fstream>
#include <vector>
#include "../utility/vec3.h"
#include "../utility/ray.h"

using namespace utility;

using namespace std;

//==============================[ GLOBALS ]====================================//

float up_left[3];
float up_right[3];
float down_right[3];
float down_left[3];

//==============================[ FILE HANDLING ]==============================//

const vector<string> split(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;
    
    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    
    return v;
}

/*
 *  Opens and read the scene text explanation file
 */
void file_handler (std::string & cena, std::string & line, std::string & name, int & width, int & height)
{
    std::ifstream arquivo_cena;

    arquivo_cena.open (cena, std::ofstream::in);

    while(std::getline (arquivo_cena,line)){
        vector<string> v{split(line, '=')};
        for(unsigned int i=0; i<v.size(); i++) {
            if(v[i] == "NAME")
                name = v[i+1];
            if(v[i] == "WIDTH")
                width = atoi(v[i+1].c_str());
            if(v[i] == "HEIGHT")
                height = atoi(v[i+1].c_str());
            if (v[i] == "UPPER_LEFT")
            {
                vector<string> w{split(v[i+1], ' ')};
                for (int j = 0; j < 3; ++j)
                {
                    up_left[j] = atof(w[j].c_str())/255;
                }                
            }
            if (v[i] == "LOWER_LEFT")
            {
                vector<string> w{split(v[i+1], ' ')};
                for (int j = 0; j < 3; ++j)
                {
                    down_left[j] = atof(w[j].c_str())/255;
                }
            }
            if (v[i] == "UPPER_RIGHT")
            {
                vector<string> w{split(v[i+1], ' ')};
                for (int j = 0; j < 3; ++j)
                {
                    up_right[j] = atof(w[j].c_str())/255;
                }
            }
            if (v[i] == "LOWER_RIGHT")
            {
                vector<string> w{split(v[i+1], ' ')};
                for (int j = 0; j < 3; ++j)
                {
                    down_right[j] = atof(w[j].c_str())/255;
                }
            }
        }
    }

    arquivo_cena.close();
}

//==============================[ COLLISIONS ]=================================//

/*  
 * Returns de Z component, if the ray hits the sphere, or infinity otherwise  
 *  
 * TODO: only works with a 2D sphere  
 */ 
float hit_sphere ( const Ray & r_, const point3 & c_, float radius_ )
{  
    // First, manipulate the 1), 2) and 3) line to create the 4) equation   
    // 1) (point - center) * (point - center) - radius² = 0     
    // 2) point == ray  // 3) ray(t) = origin + direction*t     
    // 4) d*d*t² + 2(origin - center)*d*t + (origin - center)*(origin - center) - radius² = 0   
    
    // Second, calculate the a, b and c from the equation 4) (a*t² + b*t + c = 0)   
    auto oc = r_.get_origin() - c_; // (origin - center)    
    
    auto a = utility::dot( r_.get_direction(), r_.get_direction() );   // d*d  
    auto b = 2 * utility::dot( oc, r_.get_direction() );               // 2(origin - center)*d
    auto c = utility::dot(oc, oc) - (radius_ * radius_);               // (origin - center)*(origin - center) - radius²

    // Third, calculate the delta (b² - 4ac)    
    auto delta = b * b - 4 * a * c;     

    // Last, return the t component if the ray hit the sphere 
    if (delta >= 0){
        auto t = (-b - sqrt(delta))/(2*a);
        return t;   
    }

    // or return infinity otherwise
    return std::numeric_limits<float>::infinity(); 
} 

//==============================[ INTERPOLATIONS ]=============================//  


/*  
 * Find the resulting color of the horizontal interpolation  
 *  
 * TODO: only work with the range [-2;2]  
 */ 
rgb horizontal_interpolation ( const Ray & r_, const rgb & left, const rgb & right)
 {     
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
rgb vertical_interpolation ( const Ray & r_, const rgb & bottom, const rgb & top)
{    
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

//==============================[ COLOR PICKING ]==============================//

/*  
 * Returns de color of the specific pixel that is hit by the ray  
 *  
 * TODO: only works with a view plane with aspect ratio 2:1
 */ 
rgb color( const Ray & r_ )
{
    rgb UPPER_LEFT (up_left[0], up_left[1], up_left[2]);
    rgb LOWER_LEFT (down_left[0], down_left[1], down_left[2]);
    rgb UPPER_RIGHT (up_right[0], up_right[1], up_right[2]);
    rgb LOWER_RIGHT (down_right[0], down_right[1], down_right[2]);

    point3 c (0,0,-1);

    auto sphere_t = hit_sphere(r_, c, 0.5);

    if(sphere_t != std::numeric_limits<float>::infinity()){

        point3 p = r_.point_at(sphere_t);

        vec3 normal = p - c;

        auto unit_normal = utility::unit_vector(normal);
        auto color_sphere = 0.5* (unit_normal + vec3(1,1,1));
        
        return color_sphere;
    }


    auto unit_ray = (r_.get_direction());

    auto unit_ray_y = unit_ray.y();
    auto unit_ray_x = unit_ray.x();

    auto t = (0.5*unit_ray_y)+0.5;
    auto u = (0.25*unit_ray_x)+0.5;

    rgb result = (LOWER_LEFT*(1-t)*(1-u) +
                    UPPER_LEFT*t*(1-u) +
                    LOWER_RIGHT*(1-t)*(u) +
                    UPPER_RIGHT*(t)*(u)
                    );

    return result; 
}

//==============================[ MAIN ]=====================================//

/*  
 * Responsible for the generation of the final image  
 *  
 * TODO: the camera is static
 * TODO: only make ASCII files
 */ 
int main( int argc, const char * argv[] )
{
    std::string cena, line, name="";
    int width=0, height=0;

    if(argc >1)
        cena = std::string(argv[1]);
    
    file_handler (cena, line, name, width, height);

    int n_cols{ width };
    int n_rows{ height };

    std::ofstream arq;
    
    std::string path = "../assets/";
    path = path + name;

    arq.open (path.c_str(), std::ofstream::trunc);

    std::stringstream ss;
    ss << "P3\n" << n_cols << " " << n_rows << "\n255\n";

    //=== Defining our 'camera'
    point3 lower_left_corner(-2, -1, -1); // lower left corner of the view plane.
    vec3 horizontal(4, 0, 0); // Horizontal dimension of the view plane.
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

            ss << ir << " " << ig << " " << ib << "\n";
                     
        }
    }
    arq << ss.str();
    arq.close();

    return 0;
}
