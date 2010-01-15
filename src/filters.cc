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

#include "ttsoot/filters.h"
#include <cmath>

void
BiQuadLP::register_params( paramMap &map, std::string prefix )
{
    map.add_param( prefix + "/cut", 
	    Parameter( &_cutoff, boost::bind( &BiQuadLP::set_cutoff, this, _1 ), 100.0, 20.0, 10000.0 ) );
    map.add_param( prefix + "/bw", 
	    Parameter( &_bw, boost::bind( &BiQuadLP::set_bw, this, _1 ), 0.1, 0.0, 2.0 ) );
}

void 
BiQuadLP::set_cutoff( float f )
{
    _cutoff = f;
    update_coeffs();
}

void 
BiQuadLP::set_bw( float b )
{
    _bw = b;
    update_coeffs();
}

void
BiQuadLP::update_coeffs() 
{
    float omega = 2.0*M_PI*_cutoff/48000.0;
    float sn = std::sin(omega);
    float cs = std::cos(omega);
    float alpha = sn * std::sinh( M_LN2 / 2.0 * _bw * omega / sn );
    float a0r = 1.0/(1.0+alpha);

    b0 = a0r * ( 1.0 - cs) * 0.5;
    b1 = a0r * ( 1.0 - cs);
    b2 = a0r * ( 1.0 - cs) * 0.5;
    a1 = a0r * ( 2.0 * cs);
    a2 = a0r * ( alpha - 1.0);
}

void
BiQuadHP::register_params( paramMap &map, std::string prefix )
{
    map.add_param( prefix + "/cut", 
	    Parameter( &_cutoff, boost::bind( &BiQuadHP::set_cutoff, this, _1 ), 100.0, 20.0, 10000.0 ) );
    map.add_param( prefix + "/bw", 
	    Parameter( &_bw, boost::bind( &BiQuadHP::set_bw, this, _1 ), 0.1, 0.0, 2.0 ) );
}

void 
BiQuadHP::set_cutoff( float f )
{
    _cutoff = f;
    update_coeffs();
}

void 
BiQuadHP::set_bw( float b )
{
    _bw = b;
    update_coeffs();
}

void
BiQuadHP::update_coeffs() 
{
    float omega = 2.0*M_PI*_cutoff/48000.0;
    float sn = std::sin(omega);
    float cs = std::cos(omega);
    float alpha = sn * std::sinh( M_LN2 / 2.0 * _bw * omega / sn );
    float a0r = 1.0/(1.0+alpha);

    b0 = a0r * ( 1.0 + cs) * 0.5;
    b1 = a0r * (-1.0 - cs);
    b2 = a0r * ( 1.0 + cs) * 0.5;
    a1 = a0r * ( 2.0 * cs);
    a2 = a0r * ( alpha - 1.0);
}
