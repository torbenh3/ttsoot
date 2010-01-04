#ifndef EFFECTS_H
#define EFFECTS_H

#include "block.h"

template<int MaxLine>
class Delay : public Block
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


#endif
