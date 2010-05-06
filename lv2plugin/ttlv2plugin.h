/*
 *  ttsoot - templated DSP graph composition library
 *
 *  Copyright (C) 2010  Torben Hohn <torbenh@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TTLV2PLUGIN_HH
#define TTLV2PLUGIN_HH

#include "ttsoot/block.h"
#include "lv2plugin/lv2plugin.h"
#include "lv2plugin/mididata.h"

#include "lv2plugin/lv2atom.h"

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

template<typename B, portmap_t * controlmap, portmap_t * bufmap >
class TTLV2MidiPlugin : public TTLV2Plugin< B, controlmap, bufmap >
{
    public:
	TTLV2MidiPlugin( double fs, const char *bundle_path, const LV2_Feature * const * features )
	    : TTLV2MidiPlugin< B, controlmap, bufmap > ( fs, bundle_path, features )
	{ }

	virtual void note_on ( uint8_t note, uint8_t vel ) {
	    printf( "note_on %d, %d \n", (uint32_t)  note, (uint32_t) vel );
	}
	virtual void note_off( uint8_t note, uint8_t vel ) {
	    printf( "note_off %d, %d \n", (uint32_t)  note, (uint32_t) vel );
	}

	void run( uint32_t nframes )
	{
	    float *out = this->ports.get_buffer( 0 );
	    LV2EventBuf *evbuf = this->ports.get_midi_buf(1);
	    for( auto i=evbuf->begin(); i != evbuf->end(); ++i )
	    {
		LV2MidiData mev = i->get<LV2MidiData>();
		if( mev.size == 3 ) {
		    if( mev.data[0] == 0x90 )
			note_on( mev.data[1], mev.data[2] );
		    if( mev.data[0] == 0x80 )
			note_off( mev.data[1], mev.data[2] );
		}
	    }
	    this->setup_params();
	    this->osc_block.prep();
	    for( int i=0; i<nframes; i++ )
		out[i] = this->osc_block.process();
	}
};



#endif
