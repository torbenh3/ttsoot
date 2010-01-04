
#include "parammap.h"
#include "block.h"

paramMap::paramMap( Block &block )
{
    block.register_params( *this, "" );
}

paramMap::paramMap()
{
}

float
paramMap::get( std::string key )
{
    return *(ref_map[key]);
}

void
paramMap::set( std::string key, float val )
{
    if (sideeffect_map.find(key) == sideeffect_map.end())
	*(ref_map[key]) = val;
    else
	sideeffect_map[key]( val );
}

float &
paramMap::get_ref( std::string key )
{
    return *(ref_map[key]);
}

void
paramMap::add_param( std::string key, float &ref )
{
    ref_map.insert( ref_map_t::value_type( key, &ref ) );
}

void
paramMap::add_param( std::string key, float &ref, boost::function<void(float)> sideffect )
{
    ref_map.insert( ref_map_t::value_type( key, &ref ) );
    sideeffect_map.insert( sideeffect_map_t::value_type( key, sideffect ) );
}

