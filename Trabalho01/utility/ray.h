#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h" // necessário para acessar vec3

using namespace utility; 

class Ray {

    private:
        point3 O; // the origin of the ray.
        vec3 D; // The ray's direction.

    public:
        //=== Alias
        typedef float real_type;

        //=== Special members
        Ray( point3 o_=point3(), vec3 d_=vec3() )
            : O( o_ )
            , D( d_ )
        { /* empty */ }

        //=== Access methods
        inline vec3 get_direction( void ) const { return D; }
        inline point3 get_origin( void ) const { return O; }
        point3 point_at( real_type t_ )  const
        {
            return O + t_ * D; // parametric equation of the ray.
        }
};

#endif
