
#include <jack/jack.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "block.h" 
#include "time.h" 

#include "process.h"

//Mixer<Gain<SinOsc>, Gain<SinOsc>, Gain<SinOsc>, Gain<SinOsc>> osc_block;

//Sequence< Gain<SinOsc>, Delay<128> > osc_block;

dsp::dsp() {
    osc_block.register_params( params, "" );
}

void 
dsp::dump_params() {

    for( auto i=params.begin(); i!=params.end(); i++ )
    {
	std::cout << i->first << std::endl;
    }
}

paramMap &
dsp::get_params() {
    return params;
}

void 
dsp::fill_channel( float * buf, jack_nframes_t nframes )
{
    int i;

    for( i=0; i<nframes; i++ ) {
	*buf = osc_block.process(); 
	buf++;
    }
}


