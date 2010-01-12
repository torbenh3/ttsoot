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

class nil_input_t {};

class GenBlock : public Block
{
    public:
	typedef nil_input_t input_t;
	typedef float output_t;
};

class FBlock : public Block
{
    public:
	typedef float input_t;
	typedef float output_t;
};
#endif
