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
#ifndef EFFECTS_H
#define EFFECTS_H

#include "block.h"

template<int MaxLine>
class Delay : public Block
{
    private:
	float _samples;
	int _phase;
	float _line[MaxLine];
    public:
	Delay() {
	    _samples = 10.0;
	    _phase = 0;
	    for( int i=0; i<MaxLine; i++ )
		_line[i] = 0.0;
	}
	inline float process( float s) {
	    int del = int(_samples);
	    _phase += 1;
	    _line[_phase&(MaxLine-1)] = s;
	    return _line[(_phase - del)&(MaxLine-1)];
	}
	inline void reset() { }

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_param( prefix + "/delay", Parameter( &_samples, 10.0, 1.0, 1023.0 ) );
	}
};


#endif
