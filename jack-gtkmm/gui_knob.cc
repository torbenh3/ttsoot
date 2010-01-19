
#include "gui_knob.h"
#include <gtkmm.h>

GUIKnob::GUIKnob( Glib::RefPtr<Gtk::Adjustment> adj )
    : adjustment(adj)
    , size(0)
    , save_x(0)
    , save_y(0)
    , state(0)
{
    if( scale_cache == 0 )
	scale_cache = new ScaleCache( "/home/torbenh/cvs/ggalan/pixmaps/new-knob02.png", 32 );

    set_events( Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
    adjustment->signal_value_changed().connect( sigc::mem_fun( *this, &Gtk::Widget::queue_draw ) );
}

bool
GUIKnob::on_expose_event( GdkEventExpose *ev )
{
    auto window = get_window();
    auto gc = Gdk::GC::create( window );
    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();
    int img = pos/range * (double) (scale_cache->num_images-1);

    window->draw_pixbuf( gc, (*scale_cache)[size], img*size,0,0,0,size,size,Gdk::RGB_DITHER_NONE, 0, 0 );

    return true;
}

void
GUIKnob::on_size_allocate( Gtk::Allocation &alloc )
{
    size = std::min( alloc.get_width(), alloc.get_height() );
    alloc.set_width ( size );
    alloc.set_height( size );
    Gtk::DrawingArea::on_size_allocate( alloc );
}

void
GUIKnob::on_size_request( Gtk::Requisition *req )
{
    req->height = 10;
    req->width = 10;
}

bool
GUIKnob::on_button_press_event( GdkEventButton *ev )
{
    state = 1;
    add_modal_grab();
    save_x = ev->x;
    save_y = ev->y;
    return true;
}

bool
GUIKnob::on_button_release_event( GdkEventButton *ev )
{
    state = 0;
    remove_modal_grab();
    return true;
}

bool
GUIKnob::on_motion_notify_event( GdkEventMotion *ev )
{
    if( state == 0 )
	return false;

    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();

    if( ev->state & Gdk::BUTTON1_MASK )
    {
	adjustment->set_value( adjustment->get_value() + (double)(save_y - ev->y) * (range/100.0) );
	save_y = ev->y;
    } else if( ev->state & Gdk::BUTTON3_MASK ) {
	double x = ev->x - size/2;
	double y = - ev->y + size/2;
	double angle = std::atan2( y, x ) / M_PI;
	if( angle < -0.5 )
	    angle += 2.0;

	angle = -(2.0/3.0) * (angle - 1.25);
	angle *= range;
	angle += adjustment->get_lower();

	adjustment->set_value( angle );
    }

    return true;
}

ScaleCache *
GUIKnob::scale_cache = 0; 
