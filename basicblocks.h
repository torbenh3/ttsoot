#ifndef BASICBLOCKS_H
#define BASICBLOCKS_H

#include "block.h"

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

template<typename T1>
class Modulate
{
    private:
	T1 t1;
    public:
	inline float process( float s ) {
	    return t1.process() * s;
	}

	inline void reset() {
	    t1.reset();
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    t1.register_params( map, prefix + "/mod" );
	}
};

extern char gain_name[];
class Gain : public Modulate<Param<gain_name>> {};


#endif
