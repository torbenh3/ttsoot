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

#include "oscilators.h"
#include <cmath>

SinOsc::SinOsc()
{
    _sin = 0.0;
    _cos = 1.0;
    _freq = 100.0;
    _omega = 100.0*2.0*M_PI/48000.0;
}

void
SinOsc::register_params( paramMap &map, std::string prefix )
{
    //map.add_param( prefix + "/omega", Parameter( &_omega, 0.05, 0.01, 1.0 ) );
    map.add_param( prefix + "/freq", Parameter( &_freq, boost::bind( &SinOsc::set_freq, this, _1 ), 100.0, 20.0, 10000.0 ) );
}

void
SinOsc::set_freq( float f )
{
    _freq = f;
    _omega = f * 2.0*M_PI/48000.0;
}

void
OmegaParm::register_params( paramMap &map, std::string prefix )
{
    //map.add_param( prefix + "/omega", Parameter( &_omega, 0.05, 0.01, 1.0 ) );
    map.add_param( prefix + "/freq", Parameter( &_freq, boost::bind( &OmegaParm::set_freq, this, _1 ), 100.0, 20.0, 10000.0 ) );
}

void
OmegaParm::set_freq( float f )
{
    _freq = f;
    _omega = f * 2.0*M_PI/48000.0;
}

QuadSinOsc::QuadSinOsc()
{
    for( int i=0; i<4; i++ ) {
    _sin.a[i] = 0.0;
    _cos.a[i] = 1.0;
    _omega.a[i] = 100.0*2.0*M_PI/48000.0*(float)i;
    _gain.a[i] = 1.0/(float)i;
    }
    _freq = 100.0;
}

void
QuadSinOsc::register_params( paramMap &map, std::string prefix )
{
    //map.add_param( prefix + "/omega", Parameter( &_omega, 0.05, 0.01, 1.0 ) );
    map.add_param( prefix + "/freq", Parameter( &_freq, boost::bind( &QuadSinOsc::set_freq, this, _1 ), 100.0, 20.0, 10000.0 ) );
}

void
QuadSinOsc::set_freq( float f )
{
    _freq = f;
    for( int i=0; i<4; i++ )
	_omega.a[i] = f * 2.0*M_PI/48000.0 * (float) i;
}

BLSawOsc::BLSawOsc()
{
    QuadOscBase::register_params( map, "/" );
    //map.dump();
}
void 
BLSawOsc::register_params( paramMap &map, std::string prefix )
{
    map.add_param( prefix + "/freq", Parameter( &_freq, boost::bind( &BLSawOsc::set_freq, this, _1 ), 100.0, 20.0, 1000.0 ) );
}

void 
BLSawOsc::set_freq( float f )
{
    map["/0/chain/0/mod/gain"].set( 1.0 );
    map["/1/chain/0/mod/gain"].set( 1.0/2.0 ); 
    map["/2/chain/0/mod/gain"].set( 1.0/3.0 );
    map["/3/chain/0/mod/gain"].set( 1.0/4.0 );

    map["/0/in/freq"].set( f );
    map["/1/in/freq"].set( f * 2.0 );
    map["/2/in/freq"].set( f * 3.0 );
    map["/3/in/freq"].set( f * 4.0 );
}
