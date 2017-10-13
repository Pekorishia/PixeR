#ifndef _SpotLight_H_
#define _SpotLight_H_

#include "light.h" 

#include "../utility/vec3.h"

class SpotLight : public Light
{
public:

	point3 origin;
	float beamAngle;
	float fallOffAngle;

	SpotLight( point3 origin_, rgb intensity_, float beamAngle_, float fallOffAngle_)
	{
		Light::intensity = intensity_;

		origin = origin_;
		beamAngle = beamAngle_;
		fallOffAngle = fallOffAngle_;
	}

	virtual vec3 getDirection(const point3 & p_) const;
	virtual vec3 getIntensity(const point3 & p_) const;

};

vec3 SpotLight::getDirection(const point3 & p_) const
{
		return p_ - origin;
}

vec3 SpotLight::getIntensity(const point3 & p_) const
{

        vec3 v = unit_vector(p_ - origin);

        float angle = std::acos(dot(v, getDirection(p_)));

        if (angle > beamAngle + fallOffAngle) 
        {
            return rgb(0, 0, 0);
        } 
        else if (angle > beamAngle) {
            float portion = 1 - ((angle - beamAngle) / fallOffAngle);
            return rgb(std::max(0.f, intensity[rgb::R] * portion),
                       std::max(0.f, intensity[rgb::G] * portion),
                       std::max(0.f, intensity[rgb::B] * portion));
        } 
        else {
            return intensity;
        }
}

#endif