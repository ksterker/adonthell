#ifndef GUI_CONVERSATION_H
#define GUI_CONVERSATION_H

#include "gui.h"
#include <adonthell/event/factory.h>
#include <adonthell/rpg/dialog.h>
#include <adonthell/base/callback.h>
#define MAX_OPTS 16

namespace gui
{
	class conversation : public scrollview
	{
	protected:
		struct answer 
		{
			int which;
			conversation* obj;
			answer(int w=-1, conversation* o=NULL): which(w), obj(o) {}
		};
		label speaker;
		label ct;
		list_layout lo;
		button* options[MAX_OPTS];
		answer answers[MAX_OPTS];
		rpg::dialog dlg;
		const rpg::dialog_line* line;
		bool update();
		void selectopt(const events::event *evt);
		int opty;
		int optcount;
		void* arg;
		unsigned int color;
        /// all event listeners registered for the widget
        events::factory EventFactory;
	public:
		conversation(rpg::character & d, const u_int16 & w, const u_int16 & h);

	    gui::ui_event *get_finished_event(void* user_data);

		virtual bool keyup(input::keyboard_event &k);
	};
};

#endif
