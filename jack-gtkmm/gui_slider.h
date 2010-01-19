#ifndef GUI_SLIDER_HH
#define GUI_SLIDER_HH

#include <gtkmm.h>


class GUISlider : public Gtk::DrawingArea
{
    private:
	Glib::RefPtr<Gtk::Adjustment> adjustment;
	int size;

	int save_x, save_y;
	int state;

    public:
	GUISlider( Glib::RefPtr<Gtk::Adjustment> adj );

    protected:
	bool on_expose_event( GdkEventExpose *event );
	void on_size_allocate( Gtk::Allocation &alloc );
	void on_size_request( Gtk::Requisition *req );
	bool on_button_press_event( GdkEventButton *ev );
	bool on_button_release_event( GdkEventButton *ev );
	bool on_motion_notify_event( GdkEventMotion *ev );
};

class GUIFramedSlider : public Gtk::Frame
{
    public:
	GUISlider slider;
	GUIFramedSlider( Glib::RefPtr<Gtk::Adjustment> adj )
	    : slider(adj)
	{
	    add(slider);
	    slider.show();
	}
};

#endif
