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

#ifndef LV2PLUGIN_HH
#define LV2PLUGIN_HH

#include <lv2.h>
#include <vector>

class LV2EventBuf;

template<typename P>
class LV2Desc : public LV2_Descriptor
{
    private:
	static LV2_Handle instantiate_aux( const LV2_Descriptor *desc, 
				       double sample_rate,
				       const char *bundle_path,
				       const LV2_Feature * const * features)
	{
	    const LV2Desc<P> * d = static_cast< const LV2Desc<P> * > (desc);
	    return d->inst( sample_rate, bundle_path, features );
	}

	static void connect_port_aux( LV2_Handle instance,
				  uint32_t port,
				  void * data_location )
	{
	    P *plug = static_cast<P *>( instance );
	    plug->connect_port( port, data_location );
	}

	static void activate_aux( LV2_Handle instance )
	{
	    P *plug = static_cast<P *>( instance );
	    plug->activate();
	}

	static void run_aux( LV2_Handle instance, uint32_t nframes )
	{
	    P *plug = static_cast<P *>( instance );
	    plug->run( nframes );
	}

	static void deactivate_aux( LV2_Handle instance )
	{
	    P *plug = static_cast<P *>( instance );
	    plug->deactivate();
	}
	static void cleanup_aux( LV2_Handle instance )
	{
	    P *plug = static_cast<P *>( instance );
	    delete plug;
	}
	static const void *extension_data_aux( const char * uri )
	{
	    return 0;
	}

	LV2_Handle inst( double sample_rate,
			 const char *bundle_path,
			 const LV2_Feature * const * features) const
	{
	    return LV2_Handle( new P( sample_rate, bundle_path, features ) );
	}

    public:
	LV2Desc( const char * uri )
	{
	    URI = uri;
	    instantiate = instantiate_aux;
	    connect_port = connect_port_aux;
	    activate = activate_aux;
	    run = run_aux;
	    deactivate = deactivate_aux;
	    cleanup = cleanup_aux;
	    extension_data = extension_data_aux;
	}
};

class PortVector : public std::vector<void *>
{
    public:
	float * get_buffer( uint32_t port ) {
	    return (float *) at(port);
	}

	LV2EventBuf * get_event_buffer( uint32_t port ) {
	    return (LV2EventBuf *) at(port);
	}

	float get_control( uint32_t port ) {
	    float *p = (float *) at(port);
	    return *p;
	}

};

class LV2Plugin
{
    protected:
	PortVector ports;

    public:
	virtual void connect_port( uint32_t port, void * data_location ) {
	    if( (port) >= ports.size() )
		ports.resize( port+1 );
	    ports[port] = data_location;
	}
	virtual void activate() {}
	virtual void run( uint32_t nframes ) = 0;
	virtual void deactivate() {}
	virtual ~LV2Plugin() {}
};

#endif
