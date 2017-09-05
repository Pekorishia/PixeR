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
        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;
};


/*
 * Returns the color of the point hitted by the ray based on the depth of the object.
 */
rgb DepthShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;

    // if hitted something
    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    {
        float t_normalized;

        // Normalize the depth of the point to the range [0;1]
        if (ht.t <= max_depth && ht.t >= min_depth){
            t_normalized = ht.t / max_depth;     
        }
        else{
            t_normalized = 1;
        }

        // Make a DERP with the depth nomalized between the foreground and background color
        rgb result = foreground*(1 - t_normalized) + background*(t_normalized);     
        return result; 
    }

    // else return the background color
    return background;
}

#endif
