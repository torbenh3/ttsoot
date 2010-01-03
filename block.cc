
#include "block.h"

SinOsc::SinOsc()
{
    _phase = 0.0;
    _omega = 100.0*2.0*M_PI/48000.0;
}

void
SinOsc::register_params( paramMap_t &map )
{
    map.insert( paramMap_t::value_type( "omega", _omega ) );
}
