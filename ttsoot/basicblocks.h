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
#ifndef BASICBLOCKS_H
#define BASICBLOCKS_H

#include "block.h"
#include "containers.h"

#include "jack/jack.h"

template<const char *Name>
class Param : public GenBlock
{
    private:
	float _param;
    public:
	Param() {
	    _param = 1.0;
	}
	inline float __attribute__((always_inline)) process()   {
	    return _param;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/" + Name, Parameter( &_param, 1.0, 0.0, 1.0 ) );
	}
};

template<int val>
class ConstInt : public GenBlock
{
    public:
	inline float __attribute__((always_inline)) process()   {
	    return (float) val;
	}
};

template<int num, int denom>
class ConstFract : public GenBlock
{
    public:
	inline float __attribute__((always_inline)) process()   {
	    return (float) num / (float) denom;
	}
};

template<typename T1>
class Modulate : public Container1<T1>
{
    public:
	typedef float input_t;
	typedef float output_t;

	inline float __attribute__((always_inline)) process( float s )   {
	    return this->t1.process() * s;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    this->t1.register_params( map, prefix + "/mod" );
	}
};

class InBuffer : public GenBlock
{
    protected:
	float * __restrict__ _buf;
	unsigned int k;

    public:
	InBuffer() {
	    _buf = 0;
	    k=0;
	}
	inline float __attribute__((always_inline)) process()   {
	    return _buf[k++];
	}

	inline void prep() {
	    k=0;
       	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_this( prefix + "/InBuf", this );
	}

	float **get_buf_ptr() { return (float **) &_buf; }
};

class JackInPort : public InBuffer
{
    private:
	jack_port_t *_port;
	jack_nframes_t _nframes;
    public:
	inline void prep() {
	    _buf = (float *) jack_port_get_buffer( _port, _nframes );
	    k=0;
	}

	void set_port_and_nframes( jack_port_t *port, jack_nframes_t nframes )
	{
	    _port = port;
	    _nframes = nframes;
	}
};

class Z1 : public FBlock
{
    private:
	float k_1;
    public:
	Z1() { k_1 = 0.0; }
	inline float __attribute__((always_inline)) process( float s )   {
	    float tmp = k_1;
	    k_1 = s;
	    return tmp;
	}
	inline void reset() { k_1 = 0.0; }
};

template<typename T1>
class Feedback : public Container1<T1>
{
    private:
	float k_1;
    public:
	typedef float input_t;
	typedef float output_t;

	inline float __attribute__((always_inline)) process( float s )   {
	    float tmp = k_1;
	    k_1 = this->t1.process( s + k_1 );
	    return tmp;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    this->t1.register_params( map, prefix + "/fb" );
	}
};

class Smooth : public FBlock
{
    private:
	float acc;
    public:
	Smooth() { acc = 0.0; }
	inline float __attribute__((always_inline)) process( float s )   {
	    acc = 0.99f*acc + 0.01f * s;
	    return acc;
	}
	inline void reset() {
	    acc = 0.0;
	}
};

class Clamp : public FBlock
{
    public:
	inline float __attribute__((always_inline)) process( float s )   {
	    return std::min( std::max( s, -1.0f ), 1.0f );
	}
};

extern char gain_name[];
class Gain : public Modulate< Param<gain_name> > {};

class SmoothGain : public Modulate< Sequence<Param<gain_name>,Smooth> > {};

class ADSR : public GenBlock
{
    private:
	float _trigger;
	float _held;

	float _att;
	float _dec;
	float _sus;
	float _rel;

	float _val;
	unsigned int _phase;
    public:
	ADSR() {
	    _trigger = 0.0;
	    _held    = 0.0;
	    _att     = 0.001;
	    _dec     = 0.001;
	    _sus     = 0.5;
	    _rel     = 0.9;
	    _val     = 0.0;
	    _phase   = 0;
	}

	inline float __attribute__((always_inline)) process()
	{
	    if( _trigger != 0.0f ) {
		_phase = 1;
		_trigger = 0.0;
	    }

	    switch( _phase ) {
		case 0:
		    return _val;
		case 1:
		    _val += _att;
		    if( _val >= 1.0f )
			_phase = 2;
		    return _val;
		case 2:
		    _val -= _dec;
		    if( _val <= _sus )
			_phase = 3;
		    return _val;
		case 3:
		    if( _held == 0.0f )
			_phase = 4;
		    return _val;
		case 4:
		    _val *= _rel;
		    if( _val <= 0.0001f ) {
			_phase = 0;
			_val = 0.0f;
		    }
		    return _val;
		default:
		    return _val;
	    }
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/att", Parameter( &_att, 0.001, 0.0001, 0.1 ) );
	    map.add_param( prefix + "/dec", Parameter( &_dec, 0.001, 0.0001, 0.1 ) );
	    map.add_param( prefix + "/sus", Parameter( &_sus, 0.5, 0.0, 1.0 ) );
	    map.add_param( prefix + "/rel", Parameter( &_rel, 0.9, 0.0001, 0.9999 ) );
	    map.add_param( prefix + "/hold", Parameter( &_held, 0.0, 0.0, 1.0 ) );
	    map.add_param( prefix + "/trig", Parameter( &_trigger, 0.001, 0.0001, 1.0 ) );
	}
};

#endif
