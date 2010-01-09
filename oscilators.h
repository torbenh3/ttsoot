#ifndef OSCILATORS_H
#define OSCILATORS_H

#include "block.h"
#include "basicblocks.h"
#include "containers.h"

class SinOsc : public Block
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

class VarSinOsc : public Block
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

class OmegaParm : public Block
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

#endif
