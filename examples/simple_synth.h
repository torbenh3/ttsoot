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
#ifndef YC20_HH
#define YC20_HH

#include <cmath>

#include "ttsoot/block.h" 
#include "ttsoot/filters.h"
#include "ttsoot/basicblocks.h"
#include "ttsoot/oscilators.h"
#include "ttsoot/effects.h"


typedef Sequence<SlowVarBLSaw,Modulate<ADSR>, SmoothGain> block_t; 

class my_engine : public Engine
{
    private:
	paramMap params;
	float **buffer;
	jack_port_t *midi_port;
	int num_pressed_keys;
	static float note2freq( int note );
    public:
	my_engine( jack_port_t *port, jack_port_t *midi_port, jack_nframes_t nframes );

	virtual paramMap & get_params();
	virtual void fill_channel( float * __restrict__ buf, jack_nframes_t nframes );
	virtual void dump_params();
};

#endif


