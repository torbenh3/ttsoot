#ifndef BASICBLOCKS_H
#define BASICBLOCKS_H

#include "block.h"
#include "containers.h"

#include "jack/jack.h"

template<const char *Name>
class Param : public Block
{
    private:
	float _param;
    public:
	Param() {
	    _param = 1.0;
	}
	inline float process() {
	    return _param;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/" + Name, Parameter( &_param, 1.0, 0.0, 1.0 ) );
	}
};

template<int val>
class ConstInt : public Block
{
    public:
	inline float process() {
	    return (float) val;
	}
};

template<int num, int denom>
class ConstFract : public Block
{
    public:
	inline float process() {
	    return (float) num / (float) denom;
	}
};

template<typename T1>
class Modulate : public Container1<T1>
{
    public:
	inline float process( float s ) {
	    return this->t1.process() * s;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    this->t1.register_params( map, prefix + "/mod" );
	}
};

class InBuffer : public Block
{
    protected:
	float * __restrict__ _buf;
	unsigned int k;

    public:
	InBuffer() {
	    _buf = 0;
	    k=0;
	}
	inline float process() {
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

class Z1 : public Block
{
    private:
	float k_1;
    public:
	Z1() { k_1 = 0.0; }
	inline float process( float s ) {
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
	inline float process( float s ) {
	    float tmp = k_1;
	    k_1 = this->t1.process( s + k_1 );
	    return tmp;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    this->t1.register_params( map, prefix + "/fb" );
	}
};

class Smooth : public Block
{
    private:
	float acc;
    public:
	Smooth() { acc = 0.0; }
	inline float process( float s ) {
	    acc = 0.99f*acc + 0.01f * s;
	    return acc;
	}
	inline void reset() {
	    acc = 0.0;
	}
};

extern char gain_name[];
class Gain : public Modulate< Param<gain_name> > {};

class SmoothGain : public Modulate< Sequence<Param<gain_name>,Smooth> > {};


#endif
