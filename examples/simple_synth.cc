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

    midi_port = midi_p;

    JackInPort *inbuf = dynamic_cast<JackInPort *>( params.get_block( "/in/InBuf" ) );
    if( inbuf )
	inbuf->set_port_and_nframes( in_port, nframes );

    num_pressed_keys = 0;
}

void 
my_engine::dump_params() {
    params.dump();
}

paramMap &
my_engine::get_params() {
    return params;
}

float
my_engine::note2freq( int note )
{
    return 440.0f * std::pow( 2.0f, (float)(note-69)/12.0f );
}

void 
my_engine::fill_channel( float * __restrict__ buf, jack_nframes_t nframes )
{
    int i;

    void *midi_buf = jack_port_get_buffer( midi_port, nframes );
    jack_nframes_t ev_cnt = jack_midi_get_event_count( midi_buf );
    for(i=0; i<ev_cnt; i++ ) {
	jack_midi_event_t ev;
	jack_midi_event_get( &ev, midi_buf, i );
	if( ev.size == 3 ) {
	    if (ev.buffer[0] == 0x90) {
		num_pressed_keys += 1;
		params["/chain/0/mod/hold"] = 1.0;
		params["/chain/0/mod/trig"] = 1.0;
		params["/in/in/freq"] = note2freq( ev.buffer[1] );
		params["/chain/1/mod/in/gain"] = (float) ev.buffer[2] / 127.0;
	    }
	    if (ev.buffer[0] == 0x80) {
		num_pressed_keys -= 1;
		if( num_pressed_keys == 0 )
		    params["/chain/0/mod/hold"] = 0.0;
	    }
	}
    }

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
