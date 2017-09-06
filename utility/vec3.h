#ifndef _VEC3_H_
#define _VEC3_H_

#include <cmath>        // sqrt, fabs
#include <iostream>     // cout, cin, endl
#include <cassert>      // assert()
#include <initializer_list> // initializer_list
#include <algorithm>    // copy
#include <iterator>     // std::begin(), std::end(), std::next()
#include <iomanip>      // setprecision()

namespace utility {
    /*!
     * Represents a 3D vector, that might be used to represent
     * points, directions, vectors, colors, offset
     */

    class vec3
    {
        public:
            //=== Aliases
            typedef float value_type;
            enum field_t { X=0, Y=1, Z=2, R=0, G=1, B=2 };

            //=== Members
            value_type e[ 3 ];

            //=== Special members
            explicit vec3( float e0_=0.f, float e1_=0.f, float e2_=0.f )
                : e{ e0_, e1_, e2_ }
            { /* empty */ }

            // Copy contructor
            vec3( const vec3 & other_ )
                : e{ other_.e[X], other_.e[Y], other_.e[Z] }
            { /* empty */ }

            vec3( std::initializer_list< value_type > il_ )
            {
                assert( il_.size() >= 3 ) ;
                std::copy( il_.begin(), std::next( il_.begin(), 3 ), std::begin(e) );
            }

            //=== Access operators
            inline value_type x() const { return e[X]; }
            inline value_type y() const { return e[Y]; }
            inline value_type z() const { return e[Z]; }
            inline value_type r() const { return e[R]; }
            inline value_type g() const { return e[G]; }
            inline value_type b() const { return e[B]; }

            // indexed access operator (rhs)
            inline value_type operator[]( size_t idx ) const { return e[ idx ]; }
            // indexed access operator (lhs)
            inline value_type& operator[]( size_t idx ) { return e[ idx ]; }

            //=== Algebraic operators

            bool operator==( const vec3 & other_ ) const
            {
                return fabs( e[X] - other_.e[X] ) < 0.00001f and
                    fabs( e[Y] - other_.e[Y] ) < 0.00001f and
                    fabs( e[Z] - other_.e[Z] ) < 0.00001f    ;
            }

            vec3 & operator=( const vec3 & other_ )
            {
                e[X] = other_.e[X];
                e[Y] = other_.e[Y];
                e[Z] = other_.e[Z];
                return *this;
            }

            vec3 & operator=(std::initializer_list< value_type > il_ )
            {
                assert( il_.size() >= 3 ) ;
                std::copy( il_.begin(), std::next( il_.begin(), 3 ), std::begin(e) );
                return *this;
            }


            // Unary '+'
            inline const vec3& operator+( void ) const { return *this; }
            // Unary '-'
            inline vec3 operator-( void ) const { return vec3( -e[X], -e[Y], -e[Z] ); }

            inline vec3& operator+=( const vec3& );
            inline vec3& operator-=( const vec3& );
            inline vec3& operator*=( const vec3& );
            inline vec3& operator/=( const vec3& );
            inline vec3& operator*=( const value_type );
            inline vec3& operator/=( const value_type );

            inline value_type length( void ) const
            {
                return sqrt( e[X]*e[X] + e[Y]*e[Y] + e[Z]*e[Z] );
            }
            inline value_type squared_length( void ) const
            {
                return ( e[X]*e[X] + e[Y]*e[Y] + e[Z]*e[Z] );
            }
            inline void make_unit_vector( void );

            inline void printRGB( void ) const {
                std::cout << "( " << e[R] << ", " << e[G] << ", " << e[B] << " )" << std::endl;
            }

            inline void printPoint( void ) const {
                std::cout << "( " << e[X] << ", " << e[Y] << ", " << e[Z] << " )" << std::endl;
            }

            inline void printVec3( void ) const {
                std::cout << "( " << e[X] << ", " << e[Y] << ", " << e[Z] << " )" << std::endl;
            }
    };

    typedef vec3 rgb;
    typedef vec3 offset;
    typedef vec3 point3;
} // namespace utility

#include "vec3.inl"

#endif
