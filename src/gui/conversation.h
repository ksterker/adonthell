#ifndef GUI_CONVERSATION_H
#define GUI_CONVERSATION_H

#include "widget.h"
#include "layout.h"
#include "label.h"
#include "button.h"
#include "rpg/dialog.h"
#include "base/callback.h"
#define MAX_OPTS 16

namespace gui
{
	class conversation:public widget
	{
	protected:
		struct answer 
		{
			int which;
			conversation* obj;
			answer(int w=-1, conversation* o=NULL): which(w), obj(o) {}
		};
		label speaker;
		layout objs;
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
		conversation(rpg::character & d, int w, int h, ::base::functor_0* e=NULL);

        virtual void draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da = NULL, gfx::surface * s = NULL) const
        { 
            widget::draw(x, y, da, s); 
            objs.draw(x, y, da, s);
        }
        
		virtual bool keyup(input::keyboard_event &k);
		virtual bool keydown(input::keyboard_event &k);
		virtual bool focus() {return objs.focus();}
	
	};
};

#endif
