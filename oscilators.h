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
#ifndef OSCILATORS_H
#define OSCILATORS_H

#include "block.h"
#include "basicblocks.h"
#include "containers.h"
#include <cmath>

class SinOsc : public GenBlock
{
    private:
	float _sin;
	float _cos;
	float _omega;
	float _freq;
    public:
	SinOsc();
	inline float process() {
	    _sin = _sin + _omega * _cos;
	    _cos = _cos - _omega * _sin;
	    return  _sin;
	}

	inline void reset() {
	    _sin = 0.0;
	    _cos = 1.0;
	}

	virtual void register_params( paramMap &map, std::string prefix );
	void set_freq( float f );
};

class VarSinOsc : public FBlock
{
    private:
	float _sin;
	float _cos;
    public:
	VarSinOsc() {
	    _sin = 0.0;
	    _cos = 1.0;
	}
	inline float process( float s ) {

	    _sin = _sin + s * _cos;
	    _cos = _cos - s * _sin;
	    return  _sin;
	}

	inline void reset() {
	    _sin = 0.0;
	    _cos = 1.0;
	}
};

class VarSinOsc_real : public Block
{
    private:
	float _phase;
    public:
	VarSinOsc_real() {
	    _phase = 0.0;
	}
	inline float process( float s ) {

	    _phase += s;
	    return  std::sin( _phase );
	}

	inline void reset() {
	    _phase = 0.0;
	}
};

class PhaseGen : public FBlock
{
    private:
	float _phase;
    public:
	PhaseGen() {
	    _phase = 0.0;
	}
	inline float process( float s ) {
	    _phase += s;
	    if( _phase >= 2.0f*M_PI )
		_phase -= 2.0f*M_PI;
	    return _phase;
	}
	inline void reset() {
	    _phase = 0.0;
	}
};

class Sin : public FBlock
{
    public:
	inline float process( float s ) {
	    return std::sin( s );
	}
};

class OmegaParm : public GenBlock
{
    private:
	float _omega;
	float _freq;

    public:
	inline float process() {
	    return _omega;
	}

	virtual void register_params( paramMap &map, std::string prefix );
	void set_freq( float f );
};

typedef float v4fs __attribute__((vector_size(16)));

typedef union { float a[4]; v4fs v; } v4f;

class QuadSinOsc : public Block
{
    private:
	v4f _sin;
	v4f _cos;
	v4f _omega;
	v4f _gain;
	float _freq;
    public:
	QuadSinOsc();
	inline float process() {
	    _sin.v += _omega.v * _cos.v;
	    _cos.v -= _omega.v * _sin.v;
	    return __builtin_ia32_vec_ext_v4sf (_sin.v, 0) 
	         + __builtin_ia32_vec_ext_v4sf (_sin.v, 1)
	         + __builtin_ia32_vec_ext_v4sf (_sin.v, 2)
	         + __builtin_ia32_vec_ext_v4sf (_sin.v, 3) ;
	}

	inline void reset() {
	    for( int i=0; i<4; i++ ) {
		_sin.a[i] = 0.0;
		_cos.a[i] = 1.0;
	    }
	}

	virtual void register_params( paramMap &map, std::string prefix );
	void set_freq( float f );
};

typedef Mixer < Sequence<SinOsc,Gain>,
		Sequence<SinOsc,Gain>,
		Sequence<SinOsc,Gain>,
		Sequence<SinOsc,Gain> > QuadOscBase;

class BLSawOsc : public QuadOscBase
{
    private:
	paramMap map;
	float _freq;
    public:
	BLSawOsc();
	virtual void register_params( paramMap &map, std::string prefix );
	void set_freq( float f );
};

typedef Parallel< Chain < Modulate<ConstInt<1>> ,VarSinOsc, Modulate <ConstFract<1,1>> >,
	          Chain < Modulate<ConstInt<2>> ,VarSinOsc, Modulate <ConstFract<1,2>> >,
	          Chain < Modulate<ConstInt<3>> ,VarSinOsc, Modulate <ConstFract<1,3>> >,
	          Chain < Modulate<ConstInt<4>> ,VarSinOsc, Modulate <ConstFract<1,4>> > > BLVarSawBase;

class VarBLSaw : public Sequence<OmegaParm,BLVarSawBase> {};

typedef Parallel< Chain < Modulate<ConstInt<1>> ,Sin, Modulate <ConstFract<1,1>> >,
	          Chain < Modulate<ConstInt<2>> ,Sin, Modulate <ConstFract<1,2>> >,
	          Chain < Modulate<ConstInt<3>> ,Sin, Modulate <ConstFract<1,3>> >,
	          Chain < Modulate<ConstInt<4>> ,Sin, Modulate <ConstFract<1,4>> > > SlowBLVarSawBase;

class SlowVarBLSaw : public Sequence<OmegaParm, PhaseGen, SlowBLVarSawBase> {};

#endif
