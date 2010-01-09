#ifndef PROCESS_HH
#define PROCESS_HH

#include <jack/jack.h>
#include "block.h"

class dsp {
    private:
	paramMap params;
	float **buffer;
    public:
	dsp( jack_port_t *port, jack_nframes_t nframes );

	paramMap & get_params();
	void fill_channel( float * __restrict__ buf, jack_nframes_t nframes );
	void dump_params();
};

#endif
