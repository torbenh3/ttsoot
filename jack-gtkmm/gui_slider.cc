
#include "gui_slider.h"

GUISlider::GUISlider( Glib::RefPtr<Gtk::Adjustment> adj )
    : adjustment(adj)
    , size(0)
    , save_x(0)
    , save_y(0)
    , state(0)
{
    set_events( Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
    adjustment->signal_value_changed().connect( sigc::mem_fun( *this, &Gtk::Widget::queue_draw ) );
}

bool
GUISlider::on_expose_event( GdkEventExpose *ev )
{
    auto window = get_window();
    auto gc = Gdk::GC::create( window );
    double range = adjustment->get_upper() - adjustment->get_lower();
    double pos   = adjustment->get_value() - adjustment->get_lower();
    int pix_hi = pos/range * (double) (get_height()-1);

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->rectangle( 0,0,get_width(),get_height()-pix_hi );
    cr->set_source_rgb( 0.6, 0.6, 0.6 );
    cr->fill();
    cr->rectangle( 0,get_height()-pix_hi,get_width(), get_height() );
    cr->set_source_rgb( 0.8, 0.8, 0.3 );
    cr->fill();

    return true;
}

void
GUISlider::on_size_allocate( Gtk::Allocation &alloc )
{
    Gtk::DrawingArea::on_size_allocate( alloc );
}

void
GUISlider::on_size_request( Gtk::Requisition *req )
{
    req->height = 30;
    req->width = 10;
}

bool
GUISlider::on_button_press_event( GdkEventButton *ev )
{
    state = 1;
    add_modal_grab();
    save_x = ev->x;
    save_y = ev->y;
    return true;
}

bool
GUISlider::on_button_release_event( GdkEventButton *ev )
{
    state = 0;
    remove_modal_grab();
    return true;
}

bool
GUISlider::on_motion_notify_event( GdkEventMotion *ev )
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

