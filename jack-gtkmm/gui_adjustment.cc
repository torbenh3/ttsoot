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

#include "gui_adjustment.h"

ParamAdjustment::ParamAdjustment( Parameter &p )
    : Gtk::Adjustment( p.def, p.min, p.max )
    , param( p )
{
    signal_value_changed().connect( sigc::mem_fun( *this, &ParamAdjustment::change_param ) );
}

void 
ParamAdjustment::change_param()
{
    param.set( (float) get_value() );
}
