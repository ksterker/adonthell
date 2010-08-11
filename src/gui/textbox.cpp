/*
 * implements a textbox
 */

#include "base/logging.h"
#include "base/utf8.h"
#include "gui/textbox.h"
#include "gui/draw.h"


namespace gui
{
    void textbox::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
		//we need to move the text over if the length is too long
		u_int32 nw, h2, offset;
		Font->getSize(txt.substr(0, insertpos), nw, h2);
		if (nw > .8 * length())
		{
            offset = nw - .8 * length();
			nw = .8 * length();
		}
		else
        {
            offset = 0;
        }
        
        Cache->scroll (offset);
		label::draw(x, y, da, target);
		if (hasfocus && (Cache->blink()) && (x+nw > 0 && x+nw < target->length()))
		{
			if (!offset)
				target->draw_line (x + Cache->ox() + nw, y + Cache->oy() + 2, 
                                   x + Cache->ox() + nw, y + Cache->oy() + Font->getSize() + 2, Font->getColor());
			else
				target->draw_line (x + nw, y + Cache->oy() + 2, 
                                   x + nw, y + Cache->oy() + Font->getSize() + 2, Font->getColor());
		}	
	}
    
	bool textbox::keydown(input::keyboard_event&k)
	{
		// lastblink = ::base::Timer.current_time();
		switch (k.key())
		{
            case input::keyboard_event::LEFT_KEY:
            {
                if (insertpos <= 0)
                {	
                    insertpos = 0;
                    return false;
                }
                else
                {
                    insertpos -= ::base::utf8::left(txt, insertpos);
                    Cache->invalidate();
                    return true;
                }
            }
            case input::keyboard_event::RIGHT_KEY:
            {
                if (insertpos > txt.size())
                {	
                    insertpos = txt.size();
                    return false;
                }
                else
                {
                    insertpos += ::base::utf8::right(txt, insertpos);
                    Cache->invalidate();
                    return true;
                }
            }
            case input::keyboard_event::BACKSPACE_KEY:
            {
                if (insertpos > 0)
                {
                    u_int32 len = ::base::utf8::left(txt, insertpos);
                    insertpos -= len;
                    txt.erase(insertpos, len);
                    Cache->invalidate();
                    return true;
                }
                break;
            }
            case input::keyboard_event::DELETE_KEY:
            {
                if (insertpos < txt.size())
                {
                    txt.erase(insertpos, ::base::utf8::right(txt, insertpos));
                    Cache->invalidate();
                    return true;
                }
                break;
            }
            case input::keyboard_event::END_KEY:
            {
                insertpos = txt.size();
                Cache->invalidate();
                return true;
            }
            case input::keyboard_event::HOME_KEY:
            {
                insertpos = 0;
                Cache->invalidate();
                return true;
            }
            default:
            {
                break;
            }
        }
		return false;
	}
    
    bool textbox::input(input::keyboard_event&k)
    {
         txt.insert(insertpos, k.unikey());
         
         Cache->invalidate();
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
				LOG(INFO) << logging::indent() << "install xclip if you want copy/paste to be functional";
			else
			{
				data[fread(data, 1, 1024, f)] = 0;
				pclose(f);
				txt = data;
				LOG(INFO) << logging::indent() << "Pasted '" << data << "'";
				insertpos = 0;
				cachevalid = false;
				return true;
			}
		}
		else
			LOG(INFO) << logging::indent() << "hello";
#endif


		
		return false;
	}
	bool textbox::mouseup(SDL_MouseButtonEvent&m)
	{
		return false;
	}
#endif

};
