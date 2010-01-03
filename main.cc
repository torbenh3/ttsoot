

#include <jack/jack.h>
#include <unistd.h>
#include "block.h" 
#include "time.h" 

jack_port_t *out_port;


    SinOsc osc_block;


int process( jack_nframes_t nframes, void *arg )
{
    int i;

    float * buf = (float *) jack_port_get_buffer( out_port, nframes );

    for( i=0; i<nframes; i++ ) {
	buf[i] = osc_block.process(); 
    }
}

int main( int argc, char **argv )
{
    jack_client_t *client = jack_client_open( "ttsoot", JackNullOption, 0 );

    out_port = jack_port_register( client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );

    jack_set_process_callback( client, process, 0 );
    jack_activate( client );

    while(1)
	sleep(1);
}
