/*
 *  ttsoot - templated DSP graph composition library
 *
 *  Copyright (C) 2010  Torben Hohn <torbenh@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef FVEC4_HH
#define FVEC4_HH

#include <xmmintrin.h>
#include <pmmintrin.h>


template<>
class fvec<4>
{
    private:
	__m128 vec;

    public:

	class fvec4_elem_ref
	{
	    private:
		fvec<4> & vec;
		unsigned int elem;
	    public:
		fvec4_elem_ref( fvec<4> & vec_ref, unsigned int e )
		  : vec( vec_ref )
		  , elem( e ) {}

		float operator= ( float f ) {
		    if( elem == 0 )
			vec.vec = _mm_set_ps( f, vec[1], vec[2], vec[3] );
		    else if( elem == 1 ) 
			vec.vec = _mm_set_ps( vec[0], f, vec[2], vec[3] );
		    else if( elem == 2 ) 
			vec.vec = _mm_set_ps( vec[0], vec[1], f, vec[3] );
		    else if( elem == 3 ) 
			vec.vec = _mm_set_ps( vec[0], vec[1], vec[2], f );
		}

		operator float () const {
		    return __builtin_ia32_vec_ext_v4sf( vec.vec, elem );
		}
	};
	class fvec4_elem_const_ref
	{
	    private:
		const fvec<4> & vec;
		unsigned int elem;
	    public:
		fvec4_elem_const_ref( const fvec<4> & vec_ref, unsigned int e )
		  : vec( vec_ref ) 
		  , elem( e ) {}

		operator float () const {
		    return __builtin_ia32_vec_ext_v4sf( vec.vec, elem );
		}
	};



	fvec<4>() {
	    vec = _mm_setzero_ps();
	}
	fvec<4>( float f ) {
	    vec = _mm_set1_ps( f );
	}
	fvec<4>( float f, fvec<3> v ) {
	    vec = _mm_set_ps( f, v[0], v[1], v[2] );
	} 
	fvec<4>( fvec<3> v, float f ) {
	    vec = _mm_set_ps(  v[0], v[1], v[2], f );
	} 
	fvec<4>( const __m128 & v ) {
	    vec = v;
	} 

	fvec4_elem_ref operator[] ( unsigned int index ) { return fvec4_elem_ref( *this, index ); }
	const fvec4_elem_const_ref operator[] ( unsigned int index ) const { return fvec4_elem_const_ref( *this, index ); }

	fvec<4> & operator*= ( float f ) {
	    vec = _mm_mul_ps( vec, _mm_set1_ps(f) );
	    return *this;
	}
	fvec<4> & operator+= ( float f ) {
	    vec = _mm_add_ps( vec, _mm_set1_ps(f) );
	    return *this;
	}
	fvec<4> & operator-= ( float f ) {
	    vec = _mm_sub_ps( vec, _mm_set1_ps(f) );
	    return *this;
	}
	fvec<4> operator+ ( float f ) {
	    return fvec<4>( *this ) += f;
	}
	fvec<4> operator* ( float f ) {
	    return fvec<4>( *this ) *= f;
	}
	fvec<4> operator- ( float f ) {
	    return fvec<4>( *this ) -= f;
	}

	fvec<4> & operator*= ( const fvec<4> & f ) {
	    vec = _mm_mul_ps( vec, f.vec );
	    return *this;
	}
	fvec<4> & operator+= ( const fvec<4> & f ) {
	    vec = _mm_add_ps( vec, f.vec );
	    return *this;
	}
	fvec<4> & operator-= ( const fvec<4> & f ) {
	    vec = _mm_sub_ps( vec, f.vec );
	    return *this;
	}
	fvec<4> operator+ ( const fvec<4> & f ) {
	    return fvec<4>( *this ) += f;
	}
	fvec<4> operator* ( const fvec<4> & f ) {
	    return fvec<4>( *this ) *= f;
	}
	fvec<4> operator- ( const fvec<4> & f ) {
	    return fvec<4>( *this ) -= f;
	}

	float sum() {
	    __m128 tmp = _mm_hadd_ps( vec, vec );
	    return _mm_cvtss_f32( _mm_hadd_ps( tmp, tmp ) ); 
	}

	fvec<4> clamp() {
	    return fvec<4>( _mm_min_ps( _mm_max_ps( vec, _mm_set1_ps( -1.0f ) ), _mm_set1_ps( 1.0f ) ) );
	}
};


#endif
