#ifndef PROCESS_HH
#define PROCESS_HH

#include <jack/jack.h>
#include "block.h"

class dsp {
    private:
	paramMap params;
	Sequence< Gain<SinOsc>, Delay<128> > osc_block;
    public:
	dsp();

	paramMap & get_params();
	void fill_channel( float * __restrict__ buf, jack_nframes_t nframes );
	void dump_params();
};

#endif
