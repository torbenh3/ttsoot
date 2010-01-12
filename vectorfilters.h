#ifndef VECTORFILTERS_HH
#define VECTORFILTERS_HH

#include "containers.h"

template<typename T1>
class VectorMul : public Container1<T1>
{
    public:
	template<int N>
	inline fvec<N> process( fvec<N> s )
	{
	    return s * this->t1.process();
	}
};

template<typename T1>
class VectorAdd : public Container1<T1>
{
    public:
	template<int N>
	inline fvec<N> process( fvec<N> s )
	{
	    return s + this->t1.process();
	}
};

class VectorSum : public Block
{
    public:
	template<int N>
	inline float process( fvec<N> s )
	{
	    return s.sum();
	}
};

template<int N>
class VectorSplit : public Block
{
    public:
	inline fvec<N> process( float s )
	{
	    return fvec<N>( s );
	}
};

template<int N>
class VGain : public Block
{
    private:
	fvec<N> _gain_vec;
    public:
	inline fvec<N> process( fvec<N> s )
	{
	    return _gain_vec * s;
	}
};

class VClamp : public Block
{
    public:
	template<int N>
	inline fvec<N> process( fvec<N> s )
	{
	    return s.clamp();
	}
};

#endif
