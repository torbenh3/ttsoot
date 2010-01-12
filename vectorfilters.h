#ifndef VECTORFILTERS_HH
#define VECTORFILTERS_HH

#include "containers.h"

template<int N, typename T1>
class VectorMul : public Container1<T1>
{
    public:
	typedef fvec<N> output_t;
	typedef fvec<N> input_t;

	inline fvec<N> process( fvec<N> s )
	{
	    return s * this->t1.process();
	}
};

template<int N, typename T1>
class VectorAdd : public Container1<T1>
{
    public:
	typedef fvec<N> output_t;
	typedef fvec<N> input_t;

	inline fvec<N> process( fvec<N> s )
	{
	    return s + this->t1.process();
	}
};

template<int N>
class VectorSum : public Block
{
    public:
	typedef float output_t;
	typedef fvec<N> input_t;

	inline float process( fvec<N> s )
	{
	    return s.sum();
	}
};

template<int N>
class VectorSplit : public Block
{
    public:
	typedef fvec<N> output_t;
	typedef float input_t;

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
	typedef fvec<N> output_t;
	typedef fvec<N> input_t;

	inline fvec<N> process( fvec<N> s )
	{
	    return _gain_vec * s;
	}
};

template<int N>
class VClamp : public Block
{
    public:
	typedef fvec<N> output_t;
	typedef fvec<N> input_t;

	inline fvec<N> process( fvec<N> s )
	{
	    return s.clamp();
	}
};

#endif
