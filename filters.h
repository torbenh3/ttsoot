#ifndef FILTERS_H
#define FILTERS_H

#include "block.h"
#include "basicblocks.h"
#include "containers.h"
#include <cmath>

class BiQuadForm1 : public Block
{
    protected:
	float b0, b1, b2;
	float a1, a2;
	float x_1;
	float x_2;
	float y_1;
	float y_2;
    public:
	BiQuadForm1()
	{
	    x_1=0; x_2=0; y_1=0; y_2=0;
       	}

	float process( float s )
	{
	    float tmp = (b0 * s) + (b1 * x_1) + (b2 * x_1) + (a1 * y_1) + (a2 * y_2);
	    x_2 = x_1;
	    x_1 = s;
	    y_2 = y_1;
	    y_1 = tmp;

	    return tmp;
	}
};

class BiQuadLP : public BiQuadForm1
{
    private:
	float _cutoff;
	float _bw;
    public:

	virtual void register_params( paramMap &map, std::string prefix );
	void update_coeffs();
	void set_cutoff( float f );
	void set_bw( float b );
};

#endif
