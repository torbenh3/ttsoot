
#include "ttm_gui.h"


TTMChannelRow::TTMChannelRow( TTMWindow & w )
    : win(w)
{
    Gtk::Button * button = Gtk::manage( new Gtk::Button( "Add" ) );
    button->signal_clicked().connect( sigc::mem_fun( *this, &TTMChannelRow::add_channel ) );
    buttonbox.add( *button );
    button->show();

    pack_start(buttonbox, Gtk::PACK_SHRINK );
    buttonbox.show();
    swin.add( swin_hbox );
    swin.set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_NEVER );
    swin_hbox.show();
    add(swin);
    swin.show();
}

void 
TTMChannelRow::add_channel()
{
    TTMChannelGUI *ch = win.create_channel();
    swin_hbox.pack_start( *ch, Gtk::PACK_SHRINK );
    ch->show();
}

Glib::ustring TTMWindow::ui_info =
    "<ui>"
    "	<toolbar name='ToolBar'>"
    "	    <toolitem action='Add'/>"
    "	</toolbar>"
    "</ui>";
