#ifndef GUI_METER_HH
#define GUI_METER_HH

#include <gtkmm.h>


class GUIMeter : public Gtk::DrawingArea
{
    private:
	Glib::RefPtr<Gtk::Adjustment> adjustment;
	int num_leds;
	int red_leds;

	int save_x, save_y;
	int state;
	int last_draw;

    public:
	GUIMeter( Glib::RefPtr<Gtk::Adjustment> adj );

    protected:
	bool on_expose_event( GdkEventExpose *event );
	void on_size_allocate( Gtk::Allocation &alloc );
	void on_size_request( Gtk::Requisition *req );
	bool on_button_press_event( GdkEventButton *ev );
	bool on_button_release_event( GdkEventButton *ev );
	bool on_motion_notify_event( GdkEventMotion *ev );

	void check_for_redraw();
};

class GUIFramedMeter : public Gtk::Frame
{
    public:
	GUIMeter meter;
	GUIFramedMeter( Glib::RefPtr<Gtk::Adjustment> adj )
	    : meter(adj)
	{
	    add(meter);
	    meter.show();
	}
};

#endif
