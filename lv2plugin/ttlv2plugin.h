#ifndef TTLV2PLUGIN_HH
#define TTLV2PLUGIN_HH

#include "lv2plugin.h"
#include "ttsoot/block.h"

typedef std::map<uint32_t,std::string> portmap_t;

template<typename B, portmap_t * controlmap, portmap_t * bufmap >
class TTLV2Plugin : public LV2Plugin
{
    private:
	B osc_block;
	paramMap pmap;

	//static std::map<uint32_t,std::string> controlmap;

	void setup_params()
	{
	    for( auto i = controlmap->begin(); i != controlmap->end(); i++ )
		pmap[i->second] = ports.get_control(i->first);
	    for( auto i = bufmap->begin(); i != bufmap->end(); i++ ) {
		InBuffer *in_buf = dynamic_cast<InBuffer *>(pmap.get_block(i->second));
		if( in_buf != 0 ) {
		    float **buf_p = in_buf->get_buf_ptr();
		    *buf_p = ports.get_buffer( i->first );
		}
	    }
	}
    public:
	TTLV2Plugin( double fs, const char *bundle_path, const LV2_Feature * const * features )
	{
	    Block::fs = fs;
	    osc_block.register_params( pmap, "" );
	    pmap.dump();
	}

	void run( uint32_t nframes )
	{
	    float *out = ports.get_buffer( 0 );
	    setup_params();
	    osc_block.prep();
	    for( int i=0; i<nframes; i++ )
		out[i] = osc_block.process();
	}
};

#endif
