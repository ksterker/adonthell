#include "conversation.h"

#define LINEHEIGHT 25 
namespace gui
{
	conversation::conversation(rpg::character & c, int w, int h, ::base::functor_1<void*>*e, void*a)
	:widget(w, h),ct(w, h/2),dlg(c),speaker(w, LINEHEIGHT),end(e),arg(a)
	{
		line = dlg.run(-1);
		ct.multiline(true);
		objs.addchild(speaker, 0, 0);
		objs.addchild(ct, 0, LINEHEIGHT);
		opty = ct.getHeight() + 30;
		optcount = 0;
		update();
	}
	void conversation::selectopt(bool down, void* arg)
	{
		answer* self = (answer*)arg;
		self->obj->line = self->obj->dlg.run(self->which);
		self->obj->update();
	}
	void conversation::update()
	{
		while (optcount)
		{
			optcount--;
			objs.removechild(*options[optcount]);
			delete options[optcount];
		}
		optcount = 0;
		if (!line) //this is null if we hit the end of the conversation
		{
			if (end) {
				(*end)(arg);
				delete end;
				end = NULL;
			}
			//speaker.setString(string(""));
			//ct.setString("(End)");
			return;
		}
		speaker.setString(string(line->speaker()) + ":");
		ct.setString(line->text());
		//*
		int i;
		int y = opty;
		optcount = line->num_answers() > MAX_OPTS ? MAX_OPTS : line->num_answers();
		for (i = 0; i < optcount; i++)
		{
			answers[i].which = i;
			answers[i].obj = this;
			options[i] = new button(w-20, LINEHEIGHT, ::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i], 2);
			options[i]->multiline(true);
			options[i]->setString(line->answer(i));
			options[i]->reheight();
			options[i]->centerV(false);
			objs.addchild(*options[i], 10, y);
			y += options[i]->getHeight() +5;
		}
		if (optcount == 0) 
		{
			optcount = 1;
			answers[0].which = -1;
			answers[0].obj = this;
			options[0] = new button(w - 20, LINEHEIGHT, ::base::make_functor(*this, &conversation::selectopt), (void*)&answers[i],2);
			options[0]->multiline(true);
			options[0]->setString("(continue)");
			options[0]->reheight();
			options[0]->centerV(false);
			objs.addchild(*options[0], 10, y);
			y += options[0]->getHeight() +5;
		}
		objs.focus();
		// */
	}
};
