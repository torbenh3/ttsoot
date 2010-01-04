#ifndef BLOCK_HH
#define BLOCK_HH

#include <cmath>
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include "parammap.h"


class Block
{
    public:
	virtual void register_params( paramMap &map, std::string prefix ) = 0;
};

class SinOsc : public Block
{
    private:
	float _sin;
	float _cos;
	float _omega;
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
};


template<typename ... Args>
class Mixer;

template<typename T1, typename ... Args>
class Mixer<T1, Args...> : public Block
{
    private:
	T1 t1;
	Mixer<Args...> t2;
    public:
	inline float process() {
	    return t1.process() + t2.process();
	}

	inline void reset() {
	    t1.reset();
	    t2.reset();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix, num+1 );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<typename T1, typename T2>
class Mixer<T1,T2> : public Block
{
    private:
	T1 t1;
	T2 t2;
    public:
	inline float process() {
	    return t1.process() + t2.process();
	}
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}

	virtual void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<typename ... Args>
class PMixer;

template<typename T1, typename ... Args>
class PMixer<T1, Args...> : public Block
{
    private:
	T1 t1;
	PMixer<Args...> t2;
    public:
	inline float process( float in) {
	    return t1.process( in ) + t2.process( in );
	}
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}

	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix, num+1 );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<typename T1, typename T2>
class PMixer<T1,T2> : public Block
{
    private:
	T1 t1;
	T2 t2;
    public:
	inline float process( float in) {
	    return t1.process( in ) + t2.process( in );
	}
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}

	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix + "/", 0 );
	}
};


template<typename ... Args> 
class Parallel;

template<typename IN, typename ... Args>
class Parallel<IN, Args...> : public Block
{
    private:
	IN in;
	PMixer<Args...> pmix;
    public:
	inline float process() {
	    return pmix.process( in.process() );
	}
	inline void reset() {
	    in.reset();
	    pmix.reset();
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    in.register_params( map, prefix + "/in" );
	    pmix.register_params( map, prefix + "/path" );
	}
};

template<typename T1, typename T2>
class Mul : public Block
{
    private:
	T1 t1;
	T2 t2;
    public:
	inline float process() {
	    return t1.process() * t2.process();
	}
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}

	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix + "/Mul/", 0 );
	}
};

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
	inline void reset() {}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/" + Name, Parameter( &_param, 1.0, 0.0, 1.0 ) );
	}
};

extern char gain_name[];
template<typename T>
class Gain : public Mul<T,Param<gain_name>> {};

template<int MaxLine>
class Delay
{
    private:
	float _samples;
	int _phase;
	float _line[MaxLine];
    public:
	Delay() {
	    _samples = 10.0;
	    _phase = 0;
	    for( int i=0; i<MaxLine; i++ )
		_line[i] = 0.0;
	}
	inline float process( float s) {
	    int del = int(_samples);
	    _phase += 1;
	    _line[_phase&(MaxLine-1)] = s;
	    return _line[(_phase - del)&(MaxLine-1)];
	}
	inline void reset() { }

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/delay", Parameter( &_samples, 10.0, 1.0, 1023.0 ) );
	}
};

template<typename ... Args>
class Chain;

template<typename T1, typename ... Args>
class Chain<T1, Args...>
{
    private:
	T1 t1;
	Chain<Args...> t2;
    public:
	inline float process( float s ) {
	    return t2.process( t1.process( s ) );
	}

	inline void reset() {
	    t1.reset();
	    t2.reset();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix + "/", 0 );
	}
};

template<typename T1>
class Chain<T1>
{
    private:
	T1 t1;
    public:
	inline float process( float s ) {
	    return t1.process( s );
	}

	inline void reset() {
	    t1.reset();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix + "/", 0 );
	}
};

template<typename ... Args>
class Sequence;

template<typename IN, typename ... Args>
class Sequence<IN, Args ...>
{
    private:
	IN in;
	Chain<Args...> t1;
    public:
	inline float process() {
	    return t1.process( in.process() );
	}

	inline void reset() {
	    in.reset();
	    t1.reset();
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    t1.register_params( map, prefix + "/chain" );
	    in.register_params( map, prefix + "/in" );
	}
};

#endif

