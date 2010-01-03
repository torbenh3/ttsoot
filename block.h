
#include <cmath>
#include <map>
#include <string>

typedef std::map<std::string, float &> paramMap_t;

class Block
{
    public:
	virtual void register_params( paramMap_t &map ) = 0;
};

class SinOsc : public Block
{
    private:
	float _phase;
	float _omega;
    public:
	SinOsc();
	inline float process() {
	    _phase = _phase + _omega;
	    return  _phase;
	}

	virtual void register_params( paramMap_t &map );
};

