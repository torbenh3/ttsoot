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
