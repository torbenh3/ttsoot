#ifndef CONTAINERS_HH
#define CONTAINERS_HH

#include <cmath>
#include <boost/lexical_cast.hpp>

#include "block.h"


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
	inline void prep() {
	    t1.prep();
	    t2.prep();
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
	inline void prep() {
	    t1.prep();
	    t2.prep();
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
	inline void prep() {
	    t1.prep();
	    t2.prep();
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
	inline void prep() {
	    t1.prep();
	    t2.prep();
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
	inline void prep() {
	    in.prep();
	    pmix.prep();
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
	inline void prep() {
	    t1.prep();
	    t2.prep();
	}

	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix + boost::lexical_cast<std::string> (num+1) );
	}
	virtual void register_params( paramMap &map, std::string prefix ) {
	    register_params( map, prefix + "/Mul/", 0 );
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
	inline void prep() {
	    t1.prep();
	    t2.prep();
	}
	void register_params( paramMap &map, std::string prefix, int num ) {
	    t1.register_params( map, prefix + boost::lexical_cast<std::string> (num) );
	    t2.register_params( map, prefix, num+1 );
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
	inline void prep() {
	    t1.prep();
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
	inline void prep() {
	    in.prep();
	    t1.prep();
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    t1.register_params( map, prefix + "/chain" );
	    in.register_params( map, prefix + "/in" );
	}
};

#endif

