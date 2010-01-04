#ifndef GUI_ADJUSTMENT_H
#define GUI_ADJUSTMENT_H

#include <gtkmm.h>
#include "parammap.h"

class ParamAdjustment : public Gtk::Adjustment
{
    private:
	Parameter &param;
    public:
	ParamAdjustment( Parameter & p );
	void change_param();
};

#endif
