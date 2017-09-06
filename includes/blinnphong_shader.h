#ifndef _BLINNPHONG_H_
#define _BLINNPHONG_H_

class BlinnphongShader : public Shader
{
	public:

		BlinnphongShader(Scene *world_)
        {
            Shader::world = world_;
        }

        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
	
};

rgb BlinnphongShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    // If the ray hitted anything
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        Ray scattered_ray;
        rgb attenuation;

        if (depth_ < 2 and ht.mat->scatter(r_, ht, attenuation, scattered_ray)
        )
            return attenuation * color(scattered_ray, t_min, t_max, depth_+1);
        else
            return rgb(0,0,0);
    }

    // Else, dye the pixel with the background color
    return Shader::vertical_interpolation(r_, rgb( 1,1,1 ), rgb( 0.5, 0.3, 0.9 ));
}

#endif