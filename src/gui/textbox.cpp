/*
 * implements a textbox
 */

#include <iostream>

#include "base/utf8.h"
#include "gui/textbox.h"
#include "gui/draw.h"
using std::cout;

/* the rate of blinking in ms */
#define BLINKRATE 500

namespace gui
{
	void textbox::draw(int x, int y, gfx::surface*s)
	{
		//we need to move the text over if the width is too long
		int nw,h2;
		f.getSize(txt.substr(0, insertpos), nw, h2);
		if (nw > .8 * w)
		{
			offset = nw - .8 * w;
			nw = .8 * w;
		}
		else
			offset = 0;
		label::draw(x,y,s);
		int now = ::base::Timer.current_time();
		if (hasfocus && (lastblink + BLINKRATE > now) && (x+nw > 0 && x+nw < s->length()))
		{
			if (!offset)
				vertLine(x+rx+nw, y + ry - f.getSize(), y + ry+2 , f.getColor(), s);
			else
				vertLine(x + nw, y + ry - f.getSize(), y + ry+2 , f.getColor(), s);
		}
		else if (lastblink + BLINKRATE*2 < now)
			lastblink = ::base::Timer.current_time();
	}
	bool textbox::keydown(input::keyboard_event&k)
	{
		lastblink = ::base::Timer.current_time();
		switch (k.key())
		{
		case input::keyboard_event::LEFT_KEY:
			if (insertpos <= 0)
			{	
				insertpos = 0;
				return false;
			}
			else
				insertpos -= ::base::utf8::left(txt, insertpos);
			cachevalid = false;
			return true;
			break;
		case input::keyboard_event::RIGHT_KEY:
			if (insertpos > txt.size())
			{	
				insertpos = txt.size();
				return false;
			}
			else
				insertpos += ::base::utf8::right(txt, insertpos);
			cachevalid = false;
			return true;
			break;
		case input::keyboard_event::BACKSPACE_KEY:
			if (insertpos > 0)
			{
                u_int32 len = ::base::utf8::left(txt, insertpos);
				insertpos -= len;
				txt.erase(insertpos, len);
				cachevalid = false;
				return true;
			}
			break;
		case input::keyboard_event::DELETE_KEY:
			if (insertpos < txt.size())
			{
				txt.erase(insertpos, ::base::utf8::right(txt, insertpos));
				cachevalid = false;
				return true;
			}
			break;
		case input::keyboard_event::END_KEY:
			insertpos = txt.size();
			cachevalid = false;
			return true;
		case input::keyboard_event::HOME_KEY:
			insertpos = 0;
			cachevalid = false;
			return true;
		case input::keyboard_event::TAB_KEY:
			return false; //this key changes the focus. not for us
		default:
			break;
		}
		return false;
	}
    
    bool textbox::input(input::keyboard_event&k)
    {
         txt.insert(insertpos, k.unikey());
         
         cachevalid = false;
         insertpos += k.unikey().length();
         return true;
    }
             
	bool textbox::keyup(input::keyboard_event&k)
	{
		return false;
	}
//waiting on mouse support
#if 0
	bool textbox::mousedown(SDL_MouseButtonEvent&m)
	{
		if (m.button == SDL_BUTTON_LEFT) //take the focus if its a left click
		{
			//figure out where the cursor should go
			int x, y;
			for (insertpos = 1; insertpos <= txt.size(); insertpos++)
			{
				f.getSize(txt.substr(0, insertpos), x, y);
				if (x-offset > m.x)
					break;
			}
			lastblink = ::base::Timer.current_time(); //turn the cursor on
			if (insertpos == txt.size() && x-offset <= m.x);
			else
				insertpos--; //position it before the character
			cachevalid = false; //redraw the textbox
			return true;
		}
#ifdef __linux__
		else if (m.button == SDL_BUTTON_MIDDLE)
		{
			char data[1024];
			FILE* f = popen("xclip -o", "r");
			if (!f)
				cout << "install xclip if you want copy/paste to be functional\n";
			else
			{
				data[fread(data, 1, 1024, f)] = 0;
				pclose(f);
				txt = data;
				cout << "Pasted '" << data << "'\n";
				insertpos = 0;
				cachevalid = false;
				return true;
			}
		}
		else
			cout << "hello\n";
#endif


		
		return false;
	}
	bool textbox::mouseup(SDL_MouseButtonEvent&m)
	{
		return false;
	}
#endif

};
