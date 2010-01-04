#ifndef PROCESS_HH
#define PROCESS_HH

#include <jack/jack.h>
#include "block.h"
#include "containers.h"
#include "effects.h"
#include "oscilators.h"
#include "basicblocks.h"

class dsp {
    private:
	paramMap params;
	Sequence< SinOsc, Gain, Delay<128> > osc_block;
    public:
	dsp();

	paramMap & get_params();
	void fill_channel( float * __restrict__ buf, jack_nframes_t nframes );
	void dump_params();
};

#endif
