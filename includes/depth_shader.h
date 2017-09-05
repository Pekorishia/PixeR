#ifndef _DEPTH_H_
#define _DEPTH_H_

#include "shader.h" 

class DepthShader : public Shader 
{

    protected:
        float min_depth;
        float max_depth;

        rgb foreground;
        rgb background;

    public:

        DepthShader(Scene *world_, float min_depth_, float max_depth_, const rgb & foreground_, const rgb & background_)
        {
            Shader::world = world_;
            min_depth = min_depth_;
            max_depth = max_depth_;
            foreground = foreground_;
            background = background_;
        }

        //=== Access methods
        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth) const;
        virtual rgb color1() const;
};

rgb DepthShader::color1() const
{
    return rgb(1,0,0);
}

rgb DepthShader::color( const Ray & r_, float t_min, float t_max, int depth) const
{
    /*HitRecord ht;

    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        float t_normalized;

        if (ht.t <= max_depth && ht.t >= min_depth){
            t_normalized = ht.t / max_depth;     
        }
        else{
            t_normalized = 1;
        }

        rgb result = foreground*(1 - t_normalized) + background*(t_normalized);     
        return result; 
    }*/

    return background;
}

#endif
