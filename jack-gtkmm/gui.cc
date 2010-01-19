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

#include "gui.h"
#include <boost/bind.hpp>
#include "gui_adjustment.h"
#include "gui_knob.h"
#include "gui_slider.h"
#include "gui_meter.h"

mainWin::mainWin( paramMap & params )
    : _params( params )
{
    for( auto i=params.begin(); i!=params.end(); i++ ) {
	Parameter &param = i->second;
	Glib::RefPtr<Gtk::Adjustment> adj = Glib::RefPtr<Gtk::Adjustment>( new ParamAdjustment( param ) );
	//std::shared_ptr<Gtk::VScale> scal( new Gtk::VScale (*adj) );
	//std::shared_ptr<GUIFramedSlider> scal( new GUIFramedSlider ( adj ) );
	std::shared_ptr<GUIFramedMeter> scal( new GUIFramedMeter ( adj ) );

	//scal->set_digits( 3 ); 
        _hbox.pack_start( *scal );
	scale_list.push_back( scal );
	scal->set_tooltip_text( i->first );
	scal->show();
    }
    add( _hbox );
    _hbox.show();
    show();
}

