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
#ifndef FVEC_HH
#define FVEC_HH

template<int N>
class fvec
{
    private:
	float _vec[N] __attribute__((aligned(16)));
    public:
	static const int size = N;

	fvec( const fvec<N> & other ) {
	    for( int i=0; i<N; i++ )
		_vec[i] = other._vec[i];
	}

	fvec() {
	    for( int i=0; i<N; i++ )
		_vec[i] = 0.0;
	}
	fvec( float f ) {
	    for( int i=0; i<N; i++ )
		_vec[i] = f;
	}
	fvec( float f, fvec<N-1> v ) {
	    _vec[0] = f;
	    for( int i=1; i<N; i++ )
		_vec[i] = v[i-1];
	}
	fvec( fvec<N-1> v, float f ) {
	    for( int i=0; i<N-1; i++ )
		_vec[i] = v[i];
	    _vec[N-1] = f;
	}

	template<int M>
	fvec( fvec<M> v1, fvec<N-M> v2 ) {
	    for( int i=0; i<(M-1); i++ )
		_vec[i] = v1[i];
	    for( int i=0; i<(N-M-1); i++ )
		_vec[i+N] = v2[i];
	}

	float & operator[] ( unsigned int index ) { return _vec[index]; }
	const float & operator[] ( unsigned int index ) const { return _vec[index]; }

	fvec<N> & operator*= ( float f ) {
	    for( int i=0; i<N; i++ )
		_vec[i] *= f;
	    return *this;
	}

	fvec<N> & operator+= ( float f ) {
	    for( int i=0; i<N; i++ )
		_vec[i] += f;
	    return *this;
	}

	fvec<N> & operator-= ( float f ) {
	    for( int i=0; i<N; i++ )
		_vec[i] -= f;
	    return *this;
	}
	fvec<N> operator+ ( float f ) {
	    return fvec<N>( *this ) += f;
	}
	fvec<N> operator* ( float f ) {
	    return fvec<N>( *this ) *= f;
	}
	fvec<N> operator- ( float f ) {
	    return fvec<N>( *this ) -= f;
	}

	fvec<N> & operator+= ( const fvec<N> & v ) {
	    for( int i=0; i<N; i++ )
		_vec[i] += v[i];
	    return *this;
	}

	fvec<N> & operator*= ( const fvec<N> & v ) {
	    for( int i=0; i<N; i++ )
		_vec[i] *= v[i];
	    return *this;
	}

	fvec<N> & operator-= ( const fvec<N> & v ) {
	    for( int i=0; i<N; i++ )
		_vec[i] -= v[i];
	    return *this;
	}

	fvec<N> operator+ ( const fvec<N> & v ) {
	    return fvec<N>( *this ) += v;
	}
	fvec<N> operator* ( const fvec<N> & v ) {
	    return fvec<N>( *this ) *= v;
	}
	fvec<N> operator- ( const fvec<N> & v ) {
	    return fvec<N>( *this ) -= v;
	}

	float sum() {
	    float acc=0.0f;
	    for( int i=0; i<N; i++ )
		acc+=_vec[i];
	    return acc;
	}

	fvec<N> clamp() {
	    fvec<N> retval;
	    for( int i=0; i<N; i++ )
		retval[i] = std::min( std::max( _vec[i], -1.0f ), 1.0f );
	    return retval;
	}
};

#endif
