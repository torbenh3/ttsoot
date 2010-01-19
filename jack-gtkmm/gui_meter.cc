
#include "gui_meter.h"

GUIMeter::GUIMeter( Glib::RefPtr<Gtk::Adjustment> adj )
    : adjustment(adj)
    , save_x(0)
    , save_y(0)
    , state(0)
    , num_leds(10)
    , red_leds(3)
    , last_draw(0)
{
    set_events( Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
    adjustment->signal_value_changed().connect( sigc::mem_fun( *this, &GUIMeter::check_for_redraw ) );
}

bool
GUIMeter::on_expose_event( GdkEventExpose *ev )
{
    auto window = get_window();
    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();
    int pix_hi = pos/range * (double) (num_leds);

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->scale( get_width(), (double) get_height() / (double) num_leds );

    int i;
    cr->set_source_rgb( 0.0, 0.8, 0.0 );
    for( i=0; i<pix_hi; i++ ) {
	if( i>=num_leds-red_leds ) {
	    cr->fill();
	    cr->set_source_rgb( 0.8, 0.0, 0.0 );
	}
	cr->rectangle( 0.15, num_leds-i-0.75, 0.7, 0.5 );
    }
    cr->fill();

    cr->set_source_rgb( 0.0, 0.3, 0.0 );
    for( ; i<num_leds; i++ ) {
	if( i>=num_leds-red_leds ) {
	    cr->fill();
	    cr->set_source_rgb( 0.3, 0.0, 0.0 );
	}
	cr->rectangle( 0.15, num_leds-i-0.75, 0.7, 0.5 );
    }
    cr->fill();
    last_draw = pix_hi;

    return true;
}

void 
GUIMeter::check_for_redraw()
{
    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();
    int pix_hi = pos/range * (double) (num_leds);
    
    if( pix_hi == last_draw )
	return;

    auto window = get_window();
    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->scale( get_width(), (double) get_height() / (double) num_leds );

    if( pix_hi > last_draw ) {
	cr->set_source_rgb( 0.0, 0.8, 0.0 );
	for( int i=last_draw; i<pix_hi; i++ ) {
	    if( i>=num_leds-red_leds ) {
		cr->fill();
		cr->set_source_rgb( 0.8, 0.0, 0.0 );
	    }
	    cr->rectangle( 0.15, num_leds-i-0.75, 0.7, 0.5 );
	}
	cr->fill();
    } else {
	cr->set_source_rgb( 0.0, 0.3, 0.0 );
	for( int i=pix_hi; i<last_draw; i++ ) {
	    if( i>=num_leds-red_leds ) {
		cr->fill();
		cr->set_source_rgb( 0.3, 0.0, 0.0 );
	    }
	    cr->rectangle( 0.15, num_leds-i-0.75, 0.7, 0.5 );
	}
	cr->fill();
    }

    last_draw = pix_hi;

}

void
GUIMeter::on_size_allocate( Gtk::Allocation &alloc )
{
    Gtk::DrawingArea::on_size_allocate( alloc );
}

void
GUIMeter::on_size_request( Gtk::Requisition *req )
{
    req->height = 30;
    req->width = 10;
}

bool
GUIMeter::on_button_press_event( GdkEventButton *ev )
{
    state = 1;
    add_modal_grab();
    save_x = ev->x;
    save_y = ev->y;
    return true;
}

bool
GUIMeter::on_button_release_event( GdkEventButton *ev )
{
    state = 0;
    remove_modal_grab();
    return true;
}

bool
GUIMeter::on_motion_notify_event( GdkEventMotion *ev )
{
    if( state == 0 )
	return false;

    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();

    if( ev->state & Gdk::BUTTON1_MASK )
    {
	adjustment->set_value( adjustment->get_value() + (double)(save_y - ev->y) * (range/500.0) );
	save_y = ev->y;
    } else if( ev->state & Gdk::BUTTON3_MASK ) {

	double y = (get_height()-ev->y)/get_height();
	y *= range;
	y += adjustment->get_lower();

	adjustment->set_value( y );
    }

    return true;
}

