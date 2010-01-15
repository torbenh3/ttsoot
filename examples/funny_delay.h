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
#include "ttsoot/effects.h"

extern char dparam_name[];
typedef VarDelay<8192, Sequence< Param<dparam_name>, Modulate<ConstInt<8191> >, Smooth> > delay_t;

typedef Sequence<JackInPort,Parallel<Feedback<Chain<delay_t, SmoothGain>>, SmoothGain>> block_t; 


class delay_engine : public Engine
{
    private:
	paramMap params;
	float **buffer;
	jack_port_t *midi_port;
    public:
	delay_engine( jack_port_t *port, jack_port_t *midi_port, jack_nframes_t nframes );

	virtual paramMap & get_params();
	virtual void fill_channel( float * __restrict__ buf, jack_nframes_t nframes );
	virtual void dump_params();
};

#endif


