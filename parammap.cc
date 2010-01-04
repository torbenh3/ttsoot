
#include "parammap.h"
#include "block.h"

Parameter::Parameter( float *ptr, float de, float mi, float ma )
    : val_ptr(ptr)
    , min(mi)
    , max(ma)
    , def(de)
{
}

Parameter::Parameter( float *ptr, setter_t set, float de, float mi, float ma )
    : val_ptr(ptr)
    , min(mi)
    , max(ma)
    , def(de)
    , setter(set)
{
}

paramMap::paramMap( Block &block )
{
    block.register_params( *this, "" );
}

paramMap::paramMap()
{
}

void
paramMap::add_param( std::string key, Parameter param )
{
    insert( value_type( key, param ) );
}

