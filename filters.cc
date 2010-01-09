
#include "filters.h"
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
