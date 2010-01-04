
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
