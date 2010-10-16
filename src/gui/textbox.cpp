/*
 Copyright (C) 2008 Rian Shelley
 Part of the Adonthell Project http://adonthell.linuxgames.com

 Adonthell is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 Adonthell is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Adonthell; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file   gui/textbox.cpp
 * @author Rian Shelley
 * @brief  Implements the textbox class.
 */

#include "base/logging.h"
#include "base/utf8.h"
#include "gui/textbox.h"

namespace gui
{
    void textbox::draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const
	{
		//we need to move the text over if the length is too long
		u_int32 nw, h2, offset;
		Font->getSize(Text.substr(0, InsertPos), nw, h2);
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
		if (HasFocus && (Cache->blink()) && (x+nw > 0 && x+nw < target->length()))
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
		Cache->blink(true);
		switch (k.key())
		{
            case input::keyboard_event::LEFT_KEY:
            {
                if (InsertPos <= 0)
                {	
                    InsertPos = 0;
                    return false;
                }
                else
                {
                    InsertPos -= ::base::utf8::left(Text, InsertPos);
                    Cache->invalidate();
                    return true;
                }
            }
            case input::keyboard_event::RIGHT_KEY:
            {
                if (InsertPos > Text.size())
                {	
                    InsertPos = Text.size();
                    return false;
                }
                else
                {
                    InsertPos += ::base::utf8::right(Text, InsertPos);
                    Cache->invalidate();
                    return true;
                }
            }
            case input::keyboard_event::BACKSPACE_KEY:
            {
                if (InsertPos > 0)
                {
                    u_int32 len = ::base::utf8::left(Text, InsertPos);
                    InsertPos -= len;
                    Text.erase(InsertPos, len);
                    Cache->invalidate();
                    return true;
                }
                break;
            }
            case input::keyboard_event::DELETE_KEY:
            {
                if (InsertPos < Text.size())
                {
                    Text.erase(InsertPos, ::base::utf8::right(Text, InsertPos));
                    Cache->invalidate();
                    return true;
                }
                break;
            }
            case input::keyboard_event::END_KEY:
            {
                InsertPos = Text.size();
                Cache->invalidate();
                return true;
            }
            case input::keyboard_event::HOME_KEY:
            {
                InsertPos = 0;
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
         Text.insert(InsertPos, k.unikey());
         
         Cache->invalidate();
         InsertPos += k.unikey().length();
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
			for (InsertPos = 1; InsertPos <= Text.size(); InsertPos++)
			{
				f.getSize(Text.substr(0, InsertPos), x, y);
				if (x-offset > m.x)
					break;
			}
			lastblink = ::base::Timer.current_time(); //turn the cursor on
			if (InsertPos == Text.size() && x-offset <= m.x);
			else
				InsertPos--; //position it before the character
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
				Text = data;
				LOG(INFO) << logging::indent() << "Pasted '" << data << "'";
				InsertPos = 0;
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
