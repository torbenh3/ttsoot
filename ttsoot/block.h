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
