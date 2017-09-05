#ifndef _SHADER_H_
#define _SHADER_H_

#include "scene.h"

#include "../utility/ray.h"

class Shader
{
public:

	virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const = 0;
	bool hit_anything( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ ) const;

    void print();

protected:

	Scene *world;

	rgb vertical_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_) const;
	rgb horizontal_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_) const;

};

void Shader::print(){
    world->list[1]->print(); 
}

bool Shader::hit_anything( const Ray & r_, float t_min_, float t_max_, HitRecord & ht_ ) const
{

    world->list[1]->print();
        
    HitRecord temp_ht;

    bool hit_anything = false;
    double closest_so_far = t_max_;

    for( int i = 0; i < world->list_size; i++){
        std::cout << "  hit_anything: ";
        world->list[i]->print();

        if (world->list[i]->hit(r_, t_min_, closest_so_far, temp_ht)){
            hit_anything = true;
            closest_so_far = temp_ht.t;
            ht_ = temp_ht;
        }
    }

    return hit_anything;
}

rgb Shader::vertical_interpolation ( const Ray & r_, const rgb & bottom_, const rgb & top_) const
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


rgb Shader::horizontal_interpolation ( const Ray & r_, const rgb & left_, const rgb & right_) const
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


#endif