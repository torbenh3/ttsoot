
#include "gui.h"
#include <boost/bind.hpp>

mainWin::mainWin( paramMap & params )
    : _params( params )
{
    for( auto i=params.begin(); i!=params.end(); i++ ) {
	std::shared_ptr<Gtk::Adjustment> adj( new Gtk::Adjustment( 0.0, 0.0, 1.0 ) );
	std::shared_ptr<Gtk::VScale> scal( new Gtk::VScale (*adj) );
	//scal.connect
        _hbox.pack_start( *scal );
	scale_list.push_back( scal );
	scal->set_has_tooltip( true );
	scal->set_tooltip_text( i->first );
	scal->show();
	adj->signal_value_changed().connect( boost::bind( &mainWin::set_param, this, i->first, adj ) );
    }
    add( _hbox );
    _hbox.show();
    show();

}

void
mainWin::set_param( std::string key, std::shared_ptr<Gtk::Adjustment> adj )
{
    _params.set( key, (float) adj->get_value() );
}

