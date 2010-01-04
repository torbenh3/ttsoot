#ifndef PARAMMAP_HH
#define PARAMMAP_HH

#include <map>
#include <string>

#include <boost/function.hpp>
#include <boost/bind.hpp>

class Block;

class paramMap
{
    private:
	typedef std::map<std::string, float *> ref_map_t;
	typedef std::map<std::string, boost::function<void(float)>> sideeffect_map_t;

	ref_map_t ref_map;
	sideeffect_map_t sideeffect_map;

    public:
	paramMap( Block & b );
	paramMap();

	ref_map_t::iterator begin() { return ref_map.begin(); }
	ref_map_t::iterator end()  { return ref_map.end(); }

	float get( std::string key );
	void set( std::string key, float val );

	float & get_ref( std::string key );

	void add_param( std::string key, float &ref );
	void add_param( std::string key, float &ref, boost::function<void(float)> sideffect );
};

#endif
