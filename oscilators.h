#ifndef OSCILATORS_H
#define OSCILATORS_H

#include "block.h"

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


#endif
