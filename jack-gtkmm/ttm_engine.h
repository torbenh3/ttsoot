#ifndef TTM_ENGINE_HH
#define TTM_ENGINE_HH

#include "rtcommand.h"
#include "jack/jack.h"
#include <string>

class TTMEngine;

class TTMChannel
{
    public:
	jack_port_t *in_port;
	TTMEngine &eng;
	float gain;
	float rms;
	float peak;
	bool  mute;
	std::string name;

	TTMChannel( TTMEngine &e, const std::string & n );
	~TTMChannel();

	void rename( const std::string & n ) {};
};

class TTMEngine
{
    private:
	jack_client_t *jack_client;

	RTCommandEngine rtcmd;
	RTList<TTMChannel> chan_list;
	jack_port_t *out_port;
	static int process_aux( jack_nframes_t nframes, void *arg );
    public:
	TTMEngine();
	~TTMEngine();

	jack_port_t * register_port( const std::string & pname );
	void unregister_port( jack_port_t *port );

	void process( jack_nframes_t nframes );
	TTMChannel * add_channel( const std::string & name );
	void rem_channel( TTMChannel *chan  );
};

#endif
