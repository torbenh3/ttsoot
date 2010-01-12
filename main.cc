

#include <jack/jack.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "time.h" 

#include "process.h"
#include "gui.h"

jack_port_t *out_port;
jack_port_t *in_port;




int process( jack_nframes_t nframes, void *arg )
{
    dsp *engine = static_cast<dsp *>( arg );

    float * buf = (float *) jack_port_get_buffer( out_port, nframes );

    engine->fill_channel( buf, nframes );

    return 0;
}

#define DUMP_SIZE 1024*20
#define BENCH_SIZE 1024
#define BENCH_ITER 1024*50
void dump( dsp &engine)
{
    float buf[DUMP_SIZE];
    engine.fill_channel( buf, DUMP_SIZE );
    for( int i=0; i<DUMP_SIZE; i++ )
	printf( "%d %f\n", i, buf[i] );
}

void bench( dsp &engine)
{
    float buf[BENCH_SIZE];
    jack_time_t start = jack_get_time();
    for( int i=0; i<BENCH_ITER; i++ )
	engine.fill_channel( buf, BENCH_SIZE );
    jack_time_t end = jack_get_time();

    float throughput = (float)(BENCH_SIZE*BENCH_ITER) / (float)(end-start);
    printf( "throughput = %f Msamples/s\n", throughput );
}

int main( int argc, char **argv )
{
    Gtk::Main kit( argc, argv );

    jack_client_t *client = jack_client_open( "ttsoot", JackNullOption, 0 );
    out_port = jack_port_register( client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
    in_port = jack_port_register( client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );
    dsp engine( in_port, jack_get_buffer_size( client ) );

    if( (argc==2) && (strcmp( argv[1], "dump" )==0) ) {

	engine.dump_params();
	//dump();
	jack_client_close( client );
	return 0;
    }
   
    if( (argc==2) && (strcmp( argv[1], "bench" )==0) ) {

	bench( engine );
	//dump();
	jack_client_close( client );
	return 0;
    }

    jack_set_process_callback( client, process, (void *) &engine );
    jack_activate( client );

    mainWin mw( engine.get_params() );
    Gtk::Main::run( mw );

    jack_client_close( client );
}
