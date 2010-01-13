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

#include "parammap.h"
#include "block.h"
#include <iostream>

Parameter::Parameter( float *ptr, float de, float mi, float ma )
    : val_ptr(ptr)
    , min(mi)
    , max(ma)
    , def(de)
{
}

Parameter::Parameter( float *ptr, setter_t set, float de, float mi, float ma )
    : val_ptr(ptr)
    , min(mi)
    , max(ma)
    , def(de)
    , setter(set)
{
}

paramMap::paramMap( Block &block )
{
    block.register_params( *this, "" );
}

paramMap::paramMap()
{
}

void
paramMap::add_param( std::string key, Parameter param )
{
    insert( value_type( key, param ) );
}

void 
paramMap::add_this( std::string key, Block *b )
{
    block_map[key] = b;
}

Block * 
paramMap::get_block( std::string key )
{
    return block_map[key];
}

void
paramMap::dump()
{
    std::cout << "--- Parameters:" << std::endl;
    for( auto i=begin(); i!=end(); i++ ) {
	std::cout << i->first << std::endl;
    }
    std::cout << "--- registered blocks:" << std::endl;
    for( auto i=block_map.begin(); i!=block_map.end(); i++ ) {
	std::cout << i->first << std::endl;
    }
}
