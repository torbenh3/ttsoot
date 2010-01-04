

#include <jack/jack.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "block.h" 
#include "time.h" 

#include "process.h"

jack_port_t *out_port;





int process( jack_nframes_t nframes, void *arg )
{

    float * buf = (float *) jack_port_get_buffer( out_port, nframes );

    fill_channel( buf, nframes );

    return 0;
}

#define DUMP_SIZE 1024*20
void dump()
{
    float buf[DUMP_SIZE];
    fill_channel( buf, DUMP_SIZE );
    for( int i=0; i<DUMP_SIZE; i++ )
	printf( "%d %f\n", i, buf[i] );
}

int main( int argc, char **argv )
{
    if( (argc==2) && (strcmp( argv[1], "dump" )==0) ) {

	dump_params();
	dump();
	return 0;
    }
   
    jack_client_t *client = jack_client_open( "ttsoot", JackNullOption, 0 );

    out_port = jack_port_register( client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );

    jack_set_process_callback( client, process, 0 );
    jack_activate( client );

    while(1)
	sleep(1);
}