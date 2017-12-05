#include "vec3.h"

namespace utility {
    //
    //=== Member function implementation.
    //
    inline void vec3::make_unit_vector( void )
    {
        /**
         * +---> unit vector.
         * |
         *
         * ^     1
         * v = _____ * v
         *     ||v||
         *
         * ||v|| = sqrt( v dot v )
         *
         *      1
         * k =_____
         *    ||v||
         */
        auto v_dot_v = e[X]*e[X] + e[Y]*e[Y] + e[Z]*e[Z] ;
        // make sure we divide by zero here.
        assert( fabs( v_dot_v - 0.f ) > 0.000001 );
        value_type k = 1.f/ sqrt( v_dot_v );
        e[X] *= k;
        e[Y] *= k;
        e[Z] *= k;
    }

    //
    //=== Non-member function implementation: operator overloading.
    //

    inline std::istream& operator>>( std::istream& is, vec3 & v )
    {
        is >> v.e[vec3::field_t::X]
            >> v.e[vec3::field_t::Y]
            >> v.e[vec3::field_t::Z];
        return is;
    }

    inline std::ostream& operator<<( std::ostream& os, const vec3 & v )
    {
        os << std::fixed << std::setprecision( 2 ) 
            << "{ "
            << v.e[vec3::field_t::X]
            << " "
            << v.e[vec3::field_t::Y]
            << " "
            << v.e[vec3::field_t::Z]
            << " }";

        return os;
    }

    inline vec3 operator+( const vec3 & v1, const vec3 & v2 )
    {
        return vec3( v1.e[vec3::X] + v2.e[vec3::X],
                v1.e[vec3::Y] + v2.e[vec3::Y],
                v1.e[vec3::Z] + v2.e[vec3::Z] );
    }

    inline vec3 operator-( const vec3 & v1, const vec3 & v2 )
    {
        return vec3( v1.e[vec3::X] - v2.e[vec3::X],
                v1.e[vec3::Y] - v2.e[vec3::Y],
                v1.e[vec3::Z] - v2.e[vec3::Z] );
    }

    inline vec3 operator*( const vec3 & v1, const vec3 & v2 )
    {
        return vec3( v1.e[vec3::X] * v2.e[vec3::X],
                v1.e[vec3::Y] * v2.e[vec3::Y],
                v1.e[vec3::Z] * v2.e[vec3::Z] );
    }

    inline vec3 operator/( const vec3 & v1, const vec3 & v2 )
    {
        return vec3( v1.e[vec3::X] / v2.e[vec3::X],
                v1.e[vec3::Y] / v2.e[vec3::Y],
                v1.e[vec3::Z] / v2.e[vec3::Z] );
    }

    inline vec3 operator*( const vec3 & v, vec3::value_type t )
    {
        return vec3( v.e[vec3::X] * t,
                v.e[vec3::Y] * t,
                v.e[vec3::Z] * t );
    }

    inline vec3 operator*( vec3::value_type t, const vec3 & v )
    {
        return vec3( v.e[vec3::X] * t,
                v.e[vec3::Y] * t,
                v.e[vec3::Z] * t );
    }

    inline vec3 operator/( const vec3 & v, vec3::value_type t )
    {
        return vec3( v.e[vec3::X] / t,
                v.e[vec3::Y] / t,
                v.e[vec3::Z] / t );
    }

    inline vec3::value_type dot( const vec3 & v1, const vec3 & v2 )
    {
        return v1.e[vec3::X] * v2.e[vec3::X] +
            v1.e[vec3::Y] * v2.e[vec3::Y] +
            v1.e[vec3::Z] * v2.e[vec3::Z] ;
    }

    inline vec3 cross( const vec3 & v1, const vec3 & v2 )
    {
        return vec3(    v1.e[vec3::Y] * v2.e[vec3::Z] - v1.e[vec3::Z] * v2.e[vec3::Y]  ,
                -( v1.e[vec3::X] * v2.e[vec3::Z] - v1.e[vec3::Z] * v2.e[vec3::X] ),
                v1.e[vec3::X] * v2.e[vec3::Y] - v1.e[vec3::Y] * v2.e[vec3::X]    );
    }

    inline vec3& vec3::operator+=( const vec3 & v )
    {
        e[X] += v.e[X];
        e[Y] += v.e[Y];
        e[Z] += v.e[Z];

        return *this;
    }

    inline vec3& vec3::operator-=( const vec3 & v )
    {
        e[X] -= v.e[X];
        e[Y] -= v.e[Y];
        e[Z] -= v.e[Z];

        return *this;
    }

    inline vec3& vec3::operator*=( const vec3 & v )
    {
        e[X] *= v.e[X];
        e[Y] *= v.e[Y];
        e[Z] *= v.e[Z];

        return *this;
    }

    inline vec3& vec3::operator/=( const vec3 & v )
    {
        e[X] /= v.e[X];
        e[Y] /= v.e[Y];
        e[Z] /= v.e[Z];

        return *this;
    }

    inline vec3& vec3::operator*=( const value_type t )
    {
        e[X] *= t;
        e[Y] *= t;
        e[Z] *= t;

        return *this;
    }

    inline vec3& vec3::operator/=( const value_type t )
    {
        assert( fabs( t - 0.f ) > 0.000001 );
        value_type k = 1.f/t;

        e[X] *= k;
        e[Y] *= k;
        e[Z] *= k;

        return *this;
    }

    inline static vec3 unit_vector( const vec3 & v )
    {
        return v / v.length() ;
    }

    inline static vec3 min_vector( const vec3 & v, const vec3 & u)
    {
        if (v.length() < u.length())
        {
            return v;
        }
        return u ;
    }

    inline static vec3 max_vector( const vec3 & v, const vec3 & u)
    {
        if (v.length() > u.length())
        {
            return v;
        }
        return u ;
    }
} // namespace utility

//==============================[ vec3.inl ]==============================//
