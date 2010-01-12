#ifndef FVEC_HH
#define FVEC_HH

template<int N>
class fvec
{
    private:
	float _vec[N] __attribute__((aligned(16)));
    public:
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

	float & operator[] ( unsigned int index ) { return _vec[index]; }

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
