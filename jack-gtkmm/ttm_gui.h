#ifndef TTM_GUI_HH
#define TTM_GUI_HH

#include <gtkmm.h>
#include <boost/lexical_cast.hpp>
#include "ttm_engine.h"

#include "gui_slider.h"
#include "gui_meter.h"
#include <cmath>

class FramedLabel : public Gtk::Label
{
    public:
	Gtk::Frame frame;
	FramedLabel( std::string text )
	    : Gtk::Label( text )
	{
	    frame.add( *this );
	    frame.show();
	}
};

class TTMChannelGUI : public Gtk::Table
{
    private:
	TTMChannel *chan;
	Gtk::Adjustment metadj;
	Gtk::Adjustment gainadj;
	GUIFramedMeter  meter;
	GUIFramedSlider gain;
	FramedLabel label_name;
	FramedLabel label_peak;
	FramedLabel label_gain;
	float peak;
    public:
	TTMChannelGUI( const std::string & name, TTMChannel *c )
	    : chan(c)
	    , Table( 20, 10, true )
	    , metadj ( -90, -90, 12 )
	    , gainadj( 0, -90, 12 )
	    , meter(Glib::RefPtr<Gtk::Adjustment>(&metadj))
	    , gain(Glib::RefPtr<Gtk::Adjustment>(&gainadj))
	    , label_name( name )
	    , label_peak( "-Inf" )
	    , label_gain( "0" )
	    , peak(0.0)
	{
	    attach( label_name.frame, 1,9,1,3 );
	    attach( label_peak.frame, 1,9,3,5 );
	    attach( meter, 5,9,6,16 );
	    attach( gain, 1,4,6,16 );
	    attach( label_gain.frame, 1,9,17,19 );

	    gainadj.signal_value_changed().connect( sigc::mem_fun( *this, &TTMChannelGUI::gain_change ) );
	    meter.show();
	    gain.show();
	    label_name.show();
	    label_peak.show();
	    label_gain.show();
	}

	void update_meter()
	{
	    float rms = chan->rms;
	    float dB;
	    if( rms == 0.0 )
		dB = -90;
	    else
		dB = std::log10( rms )*20.0;

	    metadj.set_value( dB );


	    if( peak != chan->peak ) {
		char cbuf[16];
		peak = chan->peak;
		dB = std::log10( peak )*10.0;
		snprintf( cbuf, 16, "%+2.2f", (double) dB );
		label_peak.set_text( cbuf );
		
	    }

	}

	void gain_change()
	{
	    float dB = gainadj.get_value();
	    float gain;
	    if( dB == -90.0 )
		gain = 0;
	    else
		gain = std::pow( 10.0, dB/10.0 );

	    chan->gain = gain;
	}
};

class TTMWindow : public Gtk::Window
{
    private:
	Gtk::VBox vbox;
	Gtk::HBox main_area;
	Glib::RefPtr<Gtk::ActionGroup> action_group;
	Glib::RefPtr<Gtk::UIManager> ui_manager;

	static Glib::ustring ui_info;
	int chan_id;
	TTMEngine &eng;
	std::list<Glib::RefPtr<TTMChannelGUI> > chan_list;

    public:
	TTMWindow( TTMEngine &e )
	    : eng(e)
	{ 
	    chan_id = 0;
	    action_group = Gtk::ActionGroup::create();
	    action_group->add( Gtk::Action::create( "Add", Gtk::Stock::ADD ),
		    sigc::mem_fun( *this, &TTMWindow::add_channel ) );
	    ui_manager = Gtk::UIManager::create();
	    ui_manager->insert_action_group( action_group );
	    ui_manager->add_ui_from_string( ui_info );

	    Gtk::Widget *toolbar = ui_manager->get_widget( "/ToolBar" );
	    vbox.add( *toolbar );
	    vbox.add( main_area );
	    toolbar->show();
	    main_area.show();

	    Glib::signal_timeout().connect( sigc::mem_fun( *this, &TTMWindow::update_all_meters ),100 );

	    add(vbox);
	    vbox.show();
	    show();
	}

	void add_channel()
       	{
	    std::string name = boost::lexical_cast<std::string>( chan_id++ );
	    
	    TTMChannel    *ch     = eng.add_channel( name );
	    TTMChannelGUI *ch_gui = new TTMChannelGUI( name, ch );
	    main_area.add(*ch_gui);
	    chan_list.push_back( Glib::RefPtr<TTMChannelGUI>(ch_gui) );
	    ch_gui->show();
	}

	bool update_all_meters()
	{
	    for( auto i=chan_list.begin(); i!=chan_list.end(); i++ )
		(*i)->update_meter();
	    return true;
	}
};

#endif
