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

#include "lv2plugin/ttlv2plugin.h"
#include <cmath>

#include "ttsoot/block.h" 
#include "ttsoot/filters.h"
#include "ttsoot/basicblocks.h"
#include "ttsoot/effects.h"

char  dparam_name[] = "delay";
typedef VarDelay<8192, Sequence< Param<dparam_name>, Modulate<ConstInt<8191> >, Smooth> > delay_t;

typedef Sequence<InBuffer,Parallel<Feedback<Chain<delay_t, SmoothGain> >, SmoothGain> > block_t; 


std::map<uint32_t,std::string> controlmap = { {2, "/chain/0/0/fb/0/0/in/delay"  },
					      {3, "/chain/0/0/fb/1/mod/in/gain" },
					      {4, "/chain/0/1/mod/in/gain"      }  };

std::map<uint32_t,std::string> bufmap = { {1, "/in/InBuf" } };

typedef LV2Desc< TTLV2Plugin< block_t, &controlmap, &bufmap > > lv2descr_t;

lv2descr_t *descriptor = 0;

LV2_SYMBOL_EXPORT const LV2_Descriptor *
lv2_descriptor( uint32_t index )
{
    if( descriptor == 0 )
	descriptor = new lv2descr_t( "http://hochstrom.endofinternet.org/ttsoot/lv2delay" );
    return descriptor;
}

