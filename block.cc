
#include "block.h"

SinOsc::SinOsc()
{
    _sin = 0.0;
    _cos = 1.0;
    _omega = 100.0*2.0*M_PI/48000.0;
}

void
SinOsc::register_params( paramMap &map, std::string prefix )
{
    map.add_param( prefix + "/omega", Parameter( &_omega, 0.05, 0.01, 1.0 ) );
}

char gain_name[] = "gain";
