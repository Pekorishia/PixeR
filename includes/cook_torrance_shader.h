#ifndef _CookTorrance_H_
#define _CookTorrance_H_

#include <math.h>  

#include "shader.h"

class CookTorranceShader : public Shader 
{

    public:

        CookTorranceShader(Scene *world_)
        {
            Shader::world = world_;
        }

        virtual rgb color(const Ray & r_, float t_min, float t_max, int depth_) const;

        float schlick(vec3 l, vec3 h, float f0) const;
    	float d(vec3 h, vec3 n, float m) const;
    	float g(vec3 l, vec3 v, vec3 h, vec3 n) const;
};

float CookTorranceShader::schlick(vec3 l, vec3 h, float f0) const {
    float cosine = std::max(0.f, dot(l, h));
    float r = (1.f - f0) / (1.f + f0);
    r = r * r;
    //return (r + (1.f - r) * std::pow((1.f - cosine), 5.f));
    return f0 + std::pow(1-cosine, 5)*(1-f0);
}

float CookTorranceShader::d(vec3 h, vec3 n, float m) const {
    float cosine = std::max(0.f, dot(n, h)) / (n.length() * h.length() ); //n]ao sei se esse calculo ta certo
	float NdotH =  dot(n, h);
    float e = -1* ( (1 - cosine*cosine) / (m*m * cosine*cosine) );
    float x = PI * m*m * cosine*cosine*cosine*cosine;


    float m_2 = m*m;
    float r1 = 1.0/ (4.0 * m_2* std::pow(NdotH, 4));
    float r2 = (NdotH * NdotH -1.0)/ (m_2 * NdotH * NdotH);
    return std::pow(1.0/(std::pow(m,2.0)*std::pow(NdotH,4.0)), ( std::pow(NdotH,2.0)-1.0)/( std::pow(m,2.0)*pow(NdotH,2.0)));
    return exp(e)/x;
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

        vec3 v = unit_vector(r_.get_direction());

        vec3 n = unit_vector(ht.normal);

        auto ambient = rgb(0.1,0,0.1)  * world->ambientLight;

        cor += ambient;

        for( int i = 0; i < Shader::world->lum_size; i++){

        	vec3 l = Shader::world->lum[i]->getDirection(ht.p);

            auto sr = Ray(ht.p, l);

            if (!Shader::hit_anything( sr, 0.001f, 1, ht_s) ){

            	l = unit_vector(l);

            	auto difuse = ht.mat->albedo->value(0,0, vec3(0,0,0));
            	
            	vec3 h = unit_vector(l + v);

			    float n_v = dot(n, v);
			    float n_l = dot(n, l);

			    if(n_l < 0)
			    	n_l *= -1;
			    if(n_v < 0)
			    	n_v *= -1;

        		float fr = ( schlick(l, h, 0.5)* d(h, n, ht.mat->m)* g(l, v, h, n) ) / 
        					(4 * (n_l) * (n_v));

        		//std::cout << " F:" << schlick(l, h, ht.mat->m) << " D:"<< d(h, n, ht.mat->m) << " G:" <<g(l, v, h, n);

        		cor += n_l * ( world->lum[i]->getIntensity() * fr + difuse);//(difuse + fr * (vec3(1,1,1) - ht.mat->albedo->value(0,0, vec3(0,0,0))));

            }

    	}
        
        return cor;
    }

    rgb result = Shader::interpolation_biline(r_);
    return result;
}

#endif