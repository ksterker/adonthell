#include "gui/conversation.h"
#include "gui/ui_event.h"
#include "event/manager.h"
#include "base/logging.h"
#include <iomanip>
#include <iostream>

#define LINEHEIGHT 20 

namespace gui
{
	conversation::conversation(rpg::character & c, const u_int16 & w, const u_int16 & h)
	: scrollview(w, h), speaker(w-30, LINEHEIGHT), ct(w-30, h/3), lo(w, h), dlg(c)
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
    
	void conversation::selectopt(const events::event *evt)
	{
	    const ui_event *e = (const ui_event *) evt;
		answer* self = (answer*) e->user_data();
		self->obj->line = self->obj->dlg.run(self->which);
		self->obj->update();
	}
    
	gui::ui_event *conversation::get_finished_event(void* user_data)
	{
	    return new ui_event(this, "finished", user_data);
	}

	bool conversation::update()
	{
	    EventFactory.clear();
		while (optcount)
		{
			optcount--;
			lo.remove_child(*options[optcount]);
			delete options[optcount];
		}
		optcount = 0;
		if (!line) //this is null if we hit the end of the conversation
		{
		    ui_event evt(this, "finished");
		    events::manager::raise_event(&evt);
		    window_manager::remove(this);
			return false;
		}
		speaker.set_string(string(line->speaker()) + ":");
		ct.set_string(line->text());
		lo.resize(layout::GROW_Y);

		int i;
		optcount = line->num_answers() > MAX_OPTS ? MAX_OPTS : line->num_answers();
         
        font *f = ct.get_font();
        f->set_color(rpg::character::get_player()->color());

		for (i = 0; i < optcount; i++)
		{
			answers[i].which = i;
			answers[i].obj = this;
			options[i] = new button(length()-40, LINEHEIGHT);
			options[i]->get_font()->set_size(LINEHEIGHT-1);
			options[i]->set_multiline(true);
			options[i]->set_style("list_item.xml");
			char tmp[16];
			snprintf(tmp, 16, "%i)", i+1);
			options[i]->set_string(string(tmp)+line->answer(i));
			options[i]->set_center(false, false);
            lo.add_child(*options[i]);

            EventFactory.register_event (options[i]->get_activate_event( (void*)&answers[i]), ::base::make_functor(*this, &conversation::selectopt));
		}
		if (optcount == 0) 
		{
			optcount = 1;
			answers[0].which = -1;
			answers[0].obj = this;
			options[0] = new button(length()-40, LINEHEIGHT);
			options[0]->get_font()->set_size(LINEHEIGHT-1);
			options[0]->set_multiline(true);
			options[0]->set_style("list_item.xml");
			options[0]->set_string("1) (continue)");
			options[0]->set_center(false, false);
			options[0]->set_color(0xffffffff);
			lo.add_child(*options[0]);

			EventFactory.register_event (options[0]->get_activate_event( (void*)&answers[0]), ::base::make_functor(*this, &conversation::selectopt));
		}

        focus();
        reset();
        return true;
	}

	bool conversation::keyup(input::keyboard_event &k) 
	{
	    int i = -1;

		switch (k.key())
		{
		case input::keyboard_event::N1_KEY:
		case input::keyboard_event::KP1_KEY:
		    i = 0;
			break;
		case input::keyboard_event::N2_KEY:
		case input::keyboard_event::KP2_KEY:
            i = 1;
			break;
		case input::keyboard_event::N3_KEY:
		case input::keyboard_event::KP3_KEY:
            i = 2;
			break;
		case input::keyboard_event::N4_KEY:
		case input::keyboard_event::KP4_KEY:
            i = 3;
			break;
		case input::keyboard_event::N5_KEY:
		case input::keyboard_event::KP5_KEY:
            i = 4;
			break;
		case input::keyboard_event::N6_KEY:
		case input::keyboard_event::KP6_KEY:
            i = 5;
			break;
		case input::keyboard_event::N7_KEY:
		case input::keyboard_event::KP7_KEY:
            i = 6;
			break;
		case input::keyboard_event::N8_KEY:
		case input::keyboard_event::KP8_KEY:
            i = 7;
			break;
		case input::keyboard_event::N9_KEY:
		case input::keyboard_event::KP9_KEY:
            i = 8;
			break;
		default:
			return layout::keyup(k);
		}

        if (optcount > i)
        {
            options[i]->activate();
            return true;
        }

        return layout::keyup(k);
	}
};
