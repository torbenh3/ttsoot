#ifndef BLOCK_H
#define BLOCK_H

#include "parammap.h"

class Block
{
    public:
	static float fs;
	virtual void register_params( paramMap &map, std::string prefix ) {}

	inline void reset() {}
	inline void prep() {}
};

#endif
