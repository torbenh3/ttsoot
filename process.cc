
#include <jack/jack.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "block.h" 
#include "time.h" 

#include "process.h"

//Sequence< BLSawOsc, Gain > osc_block;
//Sequence< QuadSinOsc, Gain > osc_block;
//typedef Sequence<JackInPort, Feedback<Chain<Z1, Z1, Z1, Z1>>, Gain> block_t;
typedef Sequence<JackInPort,Parallel<Feedback<Chain<Delay<1024>, Gain>>, Gain>> block_t;



block_t osc_block __attribute__((restrict));


dsp::dsp( jack_port_t *in_port, jack_nframes_t nframes ) {
    osc_block.register_params( params, "" );
    //InBuffer *inbuf = dynamic_cast<InBuffer *>( params.get_block( "/in/InBuf" ) );
    //buffer = inbuf->get_buf_ptr();
    //*buffer = new float[1024];
    JackInPort *inbuf = dynamic_cast<JackInPort *>( params.get_block( "/in/InBuf" ) );
    inbuf->set_port_and_nframes( in_port, nframes );
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

    osc_block.prep();
    for( i=0; i<nframes; i++ ) {
	*buf = osc_block.process(); 
	buf++;
    }
}


