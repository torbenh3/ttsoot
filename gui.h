
#include <memory>
#include <gtkmm.h>
#include "parammap.h"


class mainWin : public Gtk::Window
{
    private:
	Gtk::HBox _hbox;

	std::list<std::shared_ptr<Gtk::VScale>> scale_list;
	paramMap &_params;

    public:
	mainWin( paramMap &params );
};
