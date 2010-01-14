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
#ifndef VECTORFILTERS_HH
#define VECTORFILTERS_HH

#include "containers.h"

template<int N, typename T1>
class VectorMul : public Container1<T1>
{
    public:
	typedef fvec<N> output_t;
	typedef fvec<N> input_t;

	inline fvec<N> __attribute__((always_inline)) process( fvec<N> s )  
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

	inline fvec<N> __attribute__((always_inline)) process( fvec<N> s )  
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

	inline float __attribute__((always_inline)) process( fvec<N> s )  
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

	inline fvec<N> __attribute__((always_inline)) process( float s )  
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

	inline fvec<N> __attribute__((always_inline)) process( fvec<N> s )  
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

	inline fvec<N> __attribute__((always_inline)) process( fvec<N> s )  
	{
	    return s.clamp();
	}
};

#endif
