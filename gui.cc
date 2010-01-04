
#include "gui.h"
#include <boost/bind.hpp>
#include "gui_adjustment.h"

mainWin::mainWin( paramMap & params )
    : _params( params )
{
    for( auto i=params.begin(); i!=params.end(); i++ ) {
	Gtk::Adjustment *adj = Gtk::manage( new ParamAdjustment( i->second ) );
	std::shared_ptr<Gtk::VScale> scal( new Gtk::VScale (*adj) );
        _hbox.pack_start( *scal );
	scale_list.push_back( scal );
	scal->set_tooltip_text( i->first );
	scal->show();
    }
    add( _hbox );
    _hbox.show();
    show();

}

