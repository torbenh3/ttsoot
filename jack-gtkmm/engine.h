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
#ifndef ENGINE_HH
#define ENGINE_HH

#include <jack/jack.h>
#include "ttsoot/block.h"

class Engine {
    public:
	virtual paramMap & get_params() = 0;
	virtual void fill_channel( float * __restrict__ buf, jack_nframes_t nframes ) = 0;
	virtual void dump_params() = 0;
};

extern Engine * get_engine( jack_port_t *port, jack_port_t *midi_port, jack_nframes_t nframes );

#endif
