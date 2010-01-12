#ifndef CONTAINERS_HH
#define CONTAINERS_HH

#include <cmath>
#include <boost/lexical_cast.hpp>

#include "block.h"
#include "fvec.h"

template<typename T1, typename T2>
class Container2 : public Block
{
    protected:
	T1 t1;
	T2 t2;
    public:
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}
	inline void prep() {
	    t1.prep();
	    t2.prep();
	}

	virtual void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<typename T1>
class Container1 : public Block
{
    protected:
	T1 t1;
    public:
	inline void reset() {
	    t1.reset();
	}
	inline void prep() {
	    t1.prep();
	}

	virtual void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<template<typename ... BaseArgs> class Base, typename ... Args>
class ChainedContainer;

template<template<typename ... BaseArgs> class Base, typename T1, typename ... Args>
class ChainedContainer<Base, T1, Args...> : public Block
{
    protected:
	T1 t1;
	Base<Args...> t2;
    public:
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}
	inline void prep() {
	    t1.prep();
	    t2.prep();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix, num+1 );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<template<typename ... BaseArgs> class Base, typename T1, typename T2>
class ChainedContainer<Base, T1, T2> : public Block
{
    protected:
	T1 t1;
	T2 t2;
    public:
	inline void reset() {
	    t1.reset();
	    t2.reset();
	}
	inline void prep() {
	    t1.prep();
	    t2.prep();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num)  );
	    t2.register_params( map, prefix + "/" + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix, 0 );
	}
};

template<typename ... Args>
class Mixer : public ChainedContainer<Mixer, Args...>
{
    public:
	inline float process() {
	    return this->t1.process() + this->t2.process();
	}
};

template<typename ... Args>
class Parallel : public ChainedContainer<Parallel, Args...>
{
    public:
	inline float process( float s ) {
	    return this->t1.process( s ) + this->t2.process( s );
	}
};

template<typename ... Args>
class Vector : public ChainedContainer<Vector, Args...>
{
    public:
	inline fvec<sizeof...(Args)> process( float s ) {
	    return fvec<sizeof...(Args)>( this->t1.process( s ),  this->t2.process( s ) );
	}
};

template<typename T1, typename T2>
class Vector<T1,T2> : public Container2<T1,T2>
{
    public:
	inline fvec<2> process( float s ) {
	    return fvec<2>( this->t2.process(s), fvec<1>( this->t1.process( s ) ) );
	}
};

template<typename T1>
class Vector<T1> : public Container1<T1>
{
    public:
	inline fvec<1> process( float s ) {
	    return fvec<1>( this->t1.process( s ) );
	}
};

template<typename ... Args>
class VectorGen : public ChainedContainer<VectorGen, Args...>
{
    public:
	inline fvec<sizeof...(Args)> process() {
	    return fvec<sizeof...(Args)>( this->t1.process(),  this->t2.process() );
	}
};

template<typename T1>
class VectorGen<T1> : public Container1<T1>
{
    public:
	inline fvec<1> process() {
	    return fvec<1>( this->t1.process() );
	}
};

template<typename ... Args>
class Chain : public ChainedContainer<Chain, Args...>
{
    public:
	inline float process( float s ) {
	    return this->t2.process( this->t1.process( s ) );
	}
	template<int N>
	inline fvec<N> process( fvec<N> s ) {
	    return this->t2.process( this->t1.process( s ) );
	}
};

template<typename T1>
class Chain<T1> : public Container1<T1>
{
    public:
	inline float process( float s ) {
	    return this->t1.process( s );
	}
};


template<typename ... Args>
class Sequence;

template<typename IN, typename ... Args>
class Sequence<IN, Args ...> : public Container2<IN, Chain<Args...> >
{
    public:
	inline float process() {
	    return this->t2.process( this->t1.process() );
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    this->t1.register_params( map, prefix + "/in" );
	    this->t2.register_params( map, prefix + "/chain" );
	}
};

template<typename ... Args>
class Cascade;

template<typename C, typename T1, typename ... Args>
class Cascade<C, T1, Args...> : public Container2<T1, Cascade<C, Args...> >
{
    protected:
	C c1;
    public:
	inline float process( float s ) {
	    return this->t1.process( s ) + this->t2.process( this->c1.process( s ) );
	}
};

template<typename C1, typename T1>
class Cascade<C1,T1> : public Container1<T1>
{
    public:
	inline float process( float s ) {
	    return this->t1.process( s );
	}
};
#endif

