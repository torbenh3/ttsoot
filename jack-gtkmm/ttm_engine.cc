
#include "ttm_engine.h"
#include <cmath>


TTMChannel::TTMChannel( TTMEngine &e, const std::string & n )
    : eng(e)
    , gain(1.0)
    , rms(0.0)
    , peak(0.0)
    , mute(false)
    , name(n)
{
    in_port = eng.register_port( n );
}

TTMChannel::~TTMChannel()
{
    eng.unregister_port( in_port );
}

TTMEngine::TTMEngine()
    : rtcmd()
    , chan_list(rtcmd)
{
    jack_client = jack_client_open( "ttm", JackNullOption, NULL );
    out_port = jack_port_register( jack_client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
    jack_set_process_callback( jack_client, TTMEngine::process_aux, this );
    jack_activate( jack_client );
}

TTMEngine::~TTMEngine()
{
    jack_client_close( jack_client );
}

int
TTMEngine::process_aux( jack_nframes_t nframes, void *arg )
{
    TTMEngine *eng = static_cast<TTMEngine *>(arg);
    eng->process(nframes);
    return 0;
}

jack_port_t *
TTMEngine::register_port( const std::string &n )
{
    return jack_port_register( jack_client, n.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );
}

void
TTMEngine::unregister_port( jack_port_t *port )
{
    jack_port_unregister( jack_client, port );
}

void
TTMEngine::process( jack_nframes_t nframes )
{
    rtcmd.process_rt_commands();

    float * __restrict__ out_buf = (float * __restrict__) jack_port_get_buffer( out_port, nframes );
    for( unsigned int k=0; k<nframes; k++ )
	out_buf[k] = 0.0;

    for( auto i=chan_list.rt_begin(); i != chan_list.rt_end(); i++ ) {
	TTMChannel *c = *i;
	float * __restrict__ src = (float * __restrict__) jack_port_get_buffer( c->in_port, nframes );
	float gain = c->gain;
	float rms = 0.0;
	float peak = c->peak;
	for( unsigned int k=0; k<nframes; k++ ) {
	    float tmp = gain * src[k];
	    out_buf[k] += tmp;
	    peak = std::max( peak, fabsf( tmp ) );
	    rms += tmp*tmp;
	}
	c->rms = 0.9 * c->rms + (rms/(float)nframes) * 0.1;
	c->peak=peak;
    }
}

TTMChannel *
TTMEngine::add_channel( const std::string & name )
{
    TTMChannel *retval = new TTMChannel( *this, name );
    chan_list.add( retval );
    return retval;
}

void
TTMEngine::rem_channel( TTMChannel *chan  )
{
    chan_list.rem( chan );
}
