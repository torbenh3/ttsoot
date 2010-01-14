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
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "block.h" 
#include "time.h" 

#include "process.h"
#include "filters.h"
#include "oscilators.h"
#include "vectorfilters.h"
//#include "fvec4.h"

#include "yc20.h"
yc20_t osc_block __attribute__((restrict));

//Sequence< BLSawOsc, Gain > osc_block;
//typedef Sequence< QuadSinOsc, Gain > block_t;
//typedef Sequence<JackInPort, Feedback<Chain<Z1, Z1, Z1, Z1>>, Gain> block_t;
//typedef Sequence<JackInPort,Parallel<Feedback<Chain<Delay<1024>, Gain>>, Gain>> block_t;

//typedef Chain< BiQuadHP, BiQuadLP, Gain> FBank;

//typedef Sequence< SlowVarBLSaw, 
//	          Cascade< FlipFlop, FBank, FBank, FBank, FBank, FBank, FBank, FBank, FBank, FBank >, 
//		  Gain											> yc20voice;

//typedef Sequence< QuadSinOsc, Gain > block_t;

//typedef Sequence<JackInPort, BiQuadLP, Gain> block_t;

//typedef Sequence< SlowVarBLSaw, Gain > block_t;

//typedef Mixer< yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice, yc20voice > block_t; 

//typedef Sequence< SinOsc, VectorSplit<4>, VGain<4>, VClamp<4>, VGain<4>, VClamp<4>, VGain<4>, VectorSum<4> > block_t;

//block_t osc_block __attribute__((restrict));


dsp::dsp( jack_port_t *in_port, jack_port_t *midi_p, jack_nframes_t nframes ) {
    osc_block.register_params( params, "" );
    //InBuffer *inbuf = dynamic_cast<InBuffer *>( params.get_block( "/in/InBuf" ) );
    //buffer = inbuf->get_buf_ptr();
    //*buffer = new float[1024];
    JackInPort *inbuf = dynamic_cast<JackInPort *>( params.get_block( "/in/InBuf" ) );
    if( inbuf )
	inbuf->set_port_and_nframes( in_port, nframes );

    midi_port = midi_p;
    yc20busbar *bb = dynamic_cast<yc20busbar *>( params.get_block( "busbar" ) );
    if( bb == 0 )
	exit(20);
    keys = &(bb->keys);
}

void 
dsp::dump_params() {
    params.dump();
}

paramMap &
dsp::get_params() {
    return params;
}

void 
dsp::fill_channel( float * __restrict__ buf, jack_nframes_t nframes )
{
    int i;

    void *midi_buf = jack_port_get_buffer( midi_port, nframes );
    jack_nframes_t ev_cnt = jack_midi_get_event_count( midi_buf );
    for(i=0; i<ev_cnt; i++ ) {
	jack_midi_event_t ev;
	jack_midi_event_get( &ev, midi_buf, i );
	if( ev.size == 3 ) {
	    if( (ev.buffer[0] == 0x90) && ev.buffer[1] < 61 ) {
		(*keys)[ev.buffer[1]] = 1.0f;
		printf( "on... %d\n", (int) ev.buffer[1] );
	    }
	    if( (ev.buffer[0] == 0x80) && ev.buffer[1] < 61 ) {
		(*keys)[ev.buffer[1]] = 0.0f;
		printf( "off... %d\n", (int) ev.buffer[1] );
	    }
	}
    }
    

    osc_block.prep();
    for( i=0; i<nframes; i++ ) {
	*buf = osc_block.process(); 
	buf++;
    }
}


