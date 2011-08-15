#include "gui/conversation.h"
#include "base/logging.h"
#include <iomanip>
#include <iostream>

#define LINEHEIGHT 20 

namespace gui
{
	conversation::conversation(rpg::character & c, const u_int16 & w, const u_int16 & h, ::base::functor_0*e)
	: scrollview(w, h), ct(w-30, h/3), lo(w, h), dlg(c), speaker(w-30, LINEHEIGHT), end(e)
	{
        Look->init("window.xml");
        set_scroll_style("scrollbar.xml");
        set_size (w, h);
        ct.get_font()->set_size(LINEHEIGHT - 1);
        speaker.get_font()->set_size(LINEHEIGHT - 1);
        set_child(lo);
		line = dlg.run(-1);
		ct.set_multiline(true);
		lo.add_child(speaker);
		lo.add_child(ct);
		opty = 40;
		optcount = 0;
		color = c.color(); 
		ct.set_color(color);
        update();
	}
    
	void conversation::selectopt(bool down, void* arg)
	{
		answer* self = (answer*)arg;
		self->obj->line = self->obj->dlg.run(self->which);
		self->obj->update();
	}
    
	bool conversation::update()
	{
		while (optcount)
		{
			optcount--;
			lo.remove_child(*options[optcount]);
			delete options[optcount];
		}
		optcount = 0;
		if (!line) //this is null if we hit the end of the conversation
		{
			if (end) {
				(*end)();
				delete end;
				end = NULL;
			}
			//speaker.setString(string(""));
			//ct.setString("(End)");
			return false;
		}
		speaker.set_string(string(line->speaker()) + ":");
		ct.set_string(line->text());
		lo.resize(layout::GROW_Y);

		int i;
		optcount = line->num_answers() > MAX_OPTS ? MAX_OPTS : line->num_answers();
         
        font *f = ct.get_font();
		//the color from here doesnt work
		unsigned int fc = rpg::character::get_player()->color();
		LOG(INFO) << logging::indent() << "color changed to 0x"
                  << std::setw(8) << std::setfill('0') << std::hex << fc;
        f->set_color(fc);

		for (i = 0; i < optcount; i++)
		{
			answers[i].which = i;
			answers[i].obj = this;
			options[i] = new button(length()-40, LINEHEIGHT);
			options[i]->set_callback(::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i]);
			options[i]->get_font()->set_size(LINEHEIGHT-1);
			options[i]->set_multiline(true);
			options[i]->set_style("list_item.xml");
			char tmp[16];
			snprintf(tmp, 16, "%i)", i+1);
			options[i]->set_string(string(tmp)+line->answer(i));
			options[i]->set_center(false, false);
            lo.add_child(*options[i]);
		}
		if (optcount == 0) 
		{
			optcount = 1;
			answers[0].which = -1;
			answers[0].obj = this;
			options[0] = new button(length()-40, LINEHEIGHT);
			options[0]->set_callback(::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i]);
			options[0]->get_font()->set_size(LINEHEIGHT-1);
			options[0]->set_multiline(true);
			options[0]->set_style("list_item.xml");
			options[0]->set_string("1) (continue)");
			options[0]->set_center(false, false);
			options[0]->set_color(0xffffffff);
			lo.add_child(*options[0]);
		}

        focus();
        reset();
        return true;
	}
	bool conversation::keyup(input::keyboard_event &k) 
	{
		switch (k.key())
		{
		case input::keyboard_event::N1_KEY:
		case input::keyboard_event::KP1_KEY:
			selectopt(true, &answers[0]);
			break;
		case input::keyboard_event::N2_KEY:
		case input::keyboard_event::KP2_KEY:
			if (optcount > 0) selectopt(true, &answers[1]);
			break;
		case input::keyboard_event::N3_KEY:
		case input::keyboard_event::KP3_KEY:
			if (optcount > 1) selectopt(true, &answers[2]);
			break;
		case input::keyboard_event::N4_KEY:
		case input::keyboard_event::KP4_KEY:
			if (optcount > 2) selectopt(true, &answers[3]);
			break;
		case input::keyboard_event::N5_KEY:
		case input::keyboard_event::KP5_KEY:
			if (optcount > 3) selectopt(true, &answers[4]);
			break;
		case input::keyboard_event::N6_KEY:
		case input::keyboard_event::KP6_KEY:
			if (optcount > 4) selectopt(true, &answers[5]);
			break;
		case input::keyboard_event::N7_KEY:
		case input::keyboard_event::KP7_KEY:
			if (optcount > 5) selectopt(true, &answers[6]);
			break;
		case input::keyboard_event::N8_KEY:
		case input::keyboard_event::KP8_KEY:
			if (optcount > 6) selectopt(true, &answers[7]);
			break;
		case input::keyboard_event::N9_KEY:
		case input::keyboard_event::KP9_KEY:
			if (optcount > 7) selectopt(true, &answers[8]);
			break;
		default:
			return layout::keyup(k);
		}
		return true;
	}
};
