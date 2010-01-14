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

	typedef T1 T1_t;
	typedef T2 T2_t;
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

	typedef T1 T1_t;
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

	typedef T1 T1_t;
	typedef Base<Args...> T2_t;
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

	typedef T1 T1_t;
	typedef T2 T2_t;
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
	typedef float output_t;
	typedef float input_t;

	inline float process() {
	    return this->t1.process() + this->t2.process();
	}
};

template<typename ... Args>
class Parallel : public ChainedContainer<Parallel, Args...>
{
    public:
	typedef float output_t;
	typedef float input_t;

	inline float process( float s ) {
	    return this->t1.process( s ) + this->t2.process( s );
	}
};

template<typename ... Args>
class Vector : public ChainedContainer<Vector, Args...>
{
    public:
	typedef fvec<sizeof...(Args)> output_t;
	typedef float input_t;

	inline fvec<sizeof...(Args)> process( float s ) {
	    return fvec<sizeof...(Args)>( this->t1.process( s ),  this->t2.process( s ) );
	}
};

template<typename T1, typename T2>
class Vector<T1,T2> : public Container2<T1,T2>
{
    public:
	typedef fvec<2> output_t;
	typedef float input_t;

	inline fvec<2> process( float s ) {
	    return fvec<2>( this->t2.process(s), fvec<1>( this->t1.process( s ) ) );
	}
};

template<typename T1>
class Vector<T1> : public Container1<T1>
{
    public:
	typedef fvec<1> output_t;
	typedef float input_t;

	inline fvec<1> process( float s ) {
	    return fvec<1>( this->t1.process( s ) );
	}
};

template<typename ... Args>
class VectorGen : public ChainedContainer<VectorGen, Args...>
{
    public:
	typedef fvec<sizeof...(Args)> output_t;
	typedef nil_input_t input_t;

	inline fvec<sizeof...(Args)> process() {
	    return fvec<sizeof...(Args)>( this->t1.process(),  this->t2.process() );
	}
};

template<typename T1>
class VectorGen<T1> : public Container1<T1>
{
    public:
	typedef fvec<1> output_t;
	typedef nil_input_t input_t;

	inline fvec<1> process() {
	    return fvec<1>( this->t1.process() );
	}
};

template<typename ... Args>
class Chain : public ChainedContainer<Chain, Args...>
{
    public:
	typedef typename ChainedContainer<Chain, Args...>::T2_t::output_t output_t;
	typedef typename ChainedContainer<Chain, Args...>::T1_t::input_t  input_t;

	inline output_t process( input_t s ) {
	    return this->t2.process( this->t1.process( s ) );
	}
};

template<typename T1>
class Chain<T1> : public Container1<T1>
{
    public:
	typedef typename Container1<T1>::T1_t::output_t output_t;
	typedef typename Container1<T1>::T1_t::input_t  input_t;

	inline output_t process( input_t s ) {
	    return this->t1.process( s );
	}
};


template<typename ... Args>
class Sequence;

template<typename IN, typename ... Args>
class Sequence<IN, Args ...> : public Container2<IN, Chain<Args...> >
{
    public:
	typedef typename Container2<IN, Chain<Args...> >::T2_t::output_t output_t;
	typedef nil_input_t  input_t;

	inline output_t process() {
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
	typedef float output_t;
	typedef float input_t;

	inline float process( float s ) {
	    return this->t1.process( s ) + this->t2.process( this->c1.process( s ) );
	}
};

template<typename C1, typename T1>
class Cascade<C1,T1> : public Container1<T1>
{
    public:
	typedef float output_t;
	typedef float input_t;

	inline float process( float s ) {
	    return this->t1.process( s );
	}
};

template<typename ... Args>
class VectorCascade;

template<typename C, typename T1, typename ... Args>
class VectorCascade<C, T1, Args...> : public Container2<T1, VectorCascade<C, Args...> >
{
    protected:
	C c1;
    public:
	typedef fvec<sizeof...(Args)> output_t;
	typedef float input_t;

	inline fvec<sizeof...(Args)> process( float s ) {
	    return fvec<sizeof...(Args)> (this->t1.process( s ), this->t2.process( this->c1.process( s ) ) );
	}
};

template<typename C1, typename T1>
class VectorCascade<C1,T1> : public Container1<T1>
{
    public:
	typedef fvec<1> output_t;
	typedef float input_t;

	inline fvec<1> process( float s ) {
	    return fvec<1> (this->t1.process( s ) );
	}
};
#endif

