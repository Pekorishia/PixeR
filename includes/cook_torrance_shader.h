#ifndef _CookTorrance_H_
#define _CookTorrance_H_

#include <math.h>
#include <string> 

#include "shader.h"

class CookTorranceShader : public Shader 
{

    public:

    	std::string distribution;

        CookTorranceShader(Scene *world_, string distribution_)
        {
            Shader::world = world_;
            distribution = distribution_;
        }

        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;

        float schlick(vec3 l, vec3 h, float f0) const;
    	float d(vec3 h, vec3 n, float m) const;
    	float g(vec3 l, vec3 v, vec3 h, vec3 n) const;
};

float CookTorranceShader::schlick(vec3 l, vec3 h, float f0) const {
    float cosine = std::max(0.f, dot(l, h));
    return f0 + std::pow(1.f-cosine, 5)*(1-f0);
}

float CookTorranceShader::d(vec3 h, vec3 n, float m) const {
	float NdotH =  dot(n, h);

	if(CookTorranceShader::distribution == "blinnphong"){
		float a = (2/(m*m)) -2;
		return ((a + 2) / 2 * PI) * pow(NdotH, a); //blinnphong
	}
	else if(CookTorranceShader::distribution == "beckmann"){

	    float e = ( (NdotH*NdotH -1) / (m*m * NdotH*NdotH) );
	    float x = 1.f/(PI * m*m * NdotH*NdotH*NdotH*NdotH);
	    return std::exp(e)*x; //beckmann
	}
	else if(CookTorranceShader::distribution == "ggx"){

	    float x = (m*m -1) * NdotH*NdotH +1;
	    return (m*m)/ (PI * x*x);
	}

	return 0.f;
    
}

float CookTorranceShader::g(vec3 l, vec3 v, vec3 h, vec3 n) const {
    float n_h = std::max(0.f, dot(n, h));
    float n_v = std::max(0.f, dot(n, v));
    float v_h = std::max(0.f, dot(v, h));
    float n_l = std::max(0.f, dot(n, l));

    float g1 = (2 * n_h * n_v)/(v_h);
    float g2 = (2 * n_h * n_l)/(v_h);

    return std::min(1.f, std::min(g1, g2));
}

rgb CookTorranceShader::color( const Ray & r_, float t_min, float t_max, int depth_) const
{
    HitRecord ht;
    HitRecord ht_s;

    if ( Shader::hit_anything( r_, t_min, t_max, ht) ) 
    { 
        rgb cor(0,0,0);

        vec3 v = unit_vector(-r_.get_direction());

        vec3 n = unit_vector(ht.normal);

        for( int i = 0; i < Shader::world->lum_size; i++){

        	vec3 l = unit_vector(Shader::world->lum[i]->getDirection(ht.p));

            auto sr = Ray(ht.p, l);

            if (!Shader::hit_anything( sr, 0.f, 1, ht_s) ){

            	rgb albedo = ht.mat->albedo->value(0,0, vec3(0,0,0));
            	
            	vec3 h = unit_vector(l + v);

			    float n_v = dot(n, v);
			    float n_l = dot(n, l);

			    if(n_l < 0)
			    	n_l *= -1;
			    if(n_v < 0)
			    	n_v *= -1;

        		float fr = ( schlick(l, h, ht.mat->ref_idx)* d(h, n, ht.mat->m)* g(l, v, h, n) ) / 
        					(4 * (n_l) * (n_v));

        		vec3 spec = n_l * fr * world->lum[i]->getIntensity();
                vec3 dif  = n_l * albedo * world->lum[i]->getIntensity() * (1.f - ht.mat->ref_idx);
                cor = dif + spec;

            }
    	}
        
        return cor;
    }

    rgb result = Shader::interpolation_biline(r_);
    return result;
}

#endif