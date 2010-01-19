#ifndef GUI_KNOB_HH
#define GUI_KNOB_HH

#include <gtkmm.h>

class ScaleCache
{
    private:
	std::map<int, Glib::RefPtr<Gdk::Pixbuf> > scale_map;
	Glib::RefPtr<Gdk::Pixbuf> source;
	int source_size;
    public:
	int num_images;

	ScaleCache( std::string source_image, int n )
	{
	    source_size = n;
	    source = Gdk::Pixbuf::create_from_file( source_image ); 
	    scale_map[n] = source;
	    num_images = source->get_width()/source_size;
	}

	Glib::RefPtr<Gdk::Pixbuf> operator[](int n) {
	    if( scale_map.find( n ) == scale_map.end() )
		scale_map[n] = source->scale_simple( num_images*n, n, Gdk::INTERP_BILINEAR );
	    return scale_map[n];
	}
};

class GUIKnob : public Gtk::DrawingArea
{
    private:
	static ScaleCache *scale_cache;
	Glib::RefPtr<Gtk::Adjustment> adjustment;
	int size;

	int save_x, save_y;
	int state;

    public:
	GUIKnob( Glib::RefPtr<Gtk::Adjustment> adj );

    protected:
	bool on_expose_event( GdkEventExpose *event );
	void on_size_allocate( Gtk::Allocation &alloc );
	void on_size_request( Gtk::Requisition *req );
	bool on_button_press_event( GdkEventButton *ev );
	bool on_button_release_event( GdkEventButton *ev );
	bool on_motion_notify_event( GdkEventMotion *ev );
};

#endif
