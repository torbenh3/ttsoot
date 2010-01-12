#ifndef PARAMMAP_HH
#define PARAMMAP_HH

#include <map>
#include <string>

#include <boost/function.hpp>
#include <boost/bind.hpp>

class Block;


class Parameter
{
    public:
	typedef boost::function<void(float)> setter_t; 

    private:
	float * val_ptr;
	setter_t setter;
    public:
	Parameter() {}
	Parameter( float *ptr, float def, float min, float max );
	Parameter( float *ptr, setter_t setter, float def, float min, float max );

	float get() { return * val_ptr; }
	void set( float val ) { if(setter) setter(val); else *val_ptr = val; }

	float min, max, def;
	Parameter & operator = ( float f ) { set(f); return *this; }
};

class paramMap : public std::map<std::string, Parameter>
{
    private:
	std::map<std::string, Block *> block_map;
    public:
	paramMap( Block & b );
	paramMap();

	void add_param( std::string key, Parameter param );
	void add_this( std::string key, Block *b );

	Block *get_block( std::string key );

	void dump();
};

#endif
