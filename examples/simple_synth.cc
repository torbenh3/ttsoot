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

#include <jack/jack.h>
#include <jack/midiport.h>

#include "jack-gtkmm/engine.h"

#include "simple_synth.h"

block_t osc_block __attribute__((restrict));

my_engine::my_engine( jack_port_t *in_port, jack_port_t *midi_p, jack_nframes_t nframes )
{
    osc_block.register_params( params, "" );

    JackInPort *inbuf = dynamic_cast<JackInPort *>( params.get_block( "/in/InBuf" ) );
    if( inbuf )
	inbuf->set_port_and_nframes( in_port, nframes );
}

void 
my_engine::dump_params() {
    params.dump();
}

paramMap &
my_engine::get_params() {
    return params;
}

void 
my_engine::fill_channel( float * __restrict__ buf, jack_nframes_t nframes )
{
    int i;

    osc_block.prep();
    for( i=0; i<nframes; i++ ) {
	*buf = osc_block.process(); 
	buf++;
    }
}

Engine *
get_engine( jack_port_t *in_port, jack_port_t *midi_p, jack_nframes_t nframes )
{
    return new my_engine( in_port, midi_p, nframes );
}

char  dparam_name[] = "delay";
