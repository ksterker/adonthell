#ifndef GUI_CONVERSATION_H
#define GUI_CONVERSATION_H

#include "gui/widget.h"
#include "gui/layout.h"
#include "gui/label.h"
#include "gui/button.h"
#include "rpg/dialog.h"
#include "base/callback.h"
#define MAX_OPTS 16

namespace gui
{
	class conversation:public layout
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
		button* options[MAX_OPTS];
		answer answers[MAX_OPTS];
		rpg::dialog dlg;
		const rpg::dialog_line* line;
		bool update();
		void selectopt(bool down, void* arg);
		int opty;
		int optcount;
		::base::functor_0 * end;
		void* arg;
		unsigned int color;
	public:
		conversation(rpg::character & d, const u_int16 & w, const u_int16 & h, ::base::functor_0* e=NULL);

		virtual bool keyup(input::keyboard_event &k);
	};
};

#endif
