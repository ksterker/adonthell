#include "layout.h"
#include "draw.h"
#include "base/logging.h"

//the rate of movement for disappearing layouts
//defined in pixels per millisecond
#define FADERATE 4

namespace gui
{
	bool layout::moveright()
	{
		int old = which;
		while (which < children.size()-1)
		{
			which++;
			if (children[which].c->focus())
			{
				children[old].c->unfocus();
				return true;
			}
		}
		which = old;
		return false;
	}
	bool layout::movedown()
	{
		//find a child willing to accept which
		int old = which;
		while (which < children.size()-1)
		{
			which ++;
			if (children[which].c->focus())
			{
				children[old].c->unfocus();
				return true;
			}
		}
		which = old;
		return false;
	}
	bool layout::moveleft()
	{
		//find a child willing to accept which
		int old = which;
		while (which > 0)
		{
			which --;
			if (children[which].c->focus())
			{
				children[old].c->unfocus();
				return true;
			}
		}
		which = old; 
		return false;
	}
	bool layout::moveup()
	{
		//find a child willing to accept which
		int old = which;
		while (which > 0)	
		{
			which --;
			if (children[which].c->focus())
			{
				children[old].c->unfocus();
				return true;
			}
		}
		which = old;
		return false;
	}
//TODO: find replacement for SDL_GetTicks()
#if 0
	void layout::dofade(int x, int y, const gfx::surface* s)
	{
		if (fading && showing && dx == 0 && dy == 0)
		{
			switch (fading)
			{
			case LEFT:
				dx = -w - x;
				break;
			case RIGHT:
				dx = s->w - x;
				break;
			case TOP:
				dy = -h - y;
				break;
			case BOTTOM:
				dy = s->h - y;
				break;
			}
		}
		switch (fading)
		{
		case LEFT:
			if (showing)
			{
				dx += FADERATE * (SDL_GetTicks() - ti);
				if (dx > 0)
				{
					dx = 0;
					fading = NONE;
				}
			}
			else
			{
				dx-= FADERATE * (SDL_GetTicks() - ti);
				if (dx + x + w < 0)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case RIGHT:
			if (showing)
			{
				dx -= FADERATE *  (SDL_GetTicks() - ti);
				if (dx < 0)
				{
					dx = 0;
					fading = NONE;
				}
			}
			else
			{
				dx+= FADERATE *  (SDL_GetTicks() - ti);
				if (dx + x > s->w)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case TOP:
			if (showing)
			{
				dy += FADERATE * (SDL_GetTicks() - ti);
				if (dy > 0)
				{
					fading = NONE;
					dy = 0;
				}
			}
			else
			{
				dy -= FADERATE * (SDL_GetTicks() - ti);
				if (dy + y + h < 0)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case BOTTOM:
			if (showing)
			{
				dy -= FADERATE * (SDL_GetTicks() - ti);
				if (dy < 0)
				{
					fading = NONE;
					dy = 0;
				}
			}
			else
			{
				dy += FADERATE *  (SDL_GetTicks() - ti);
				if (dy + y > s->h)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		}
		ti = SDL_GetTicks();
	}
#endif

	void layout::draw(int x, int y, gfx::surface* s)
	{
		//process the fading routines
		//dofade(x, y, s);
		if (!visible)
			return;
		
		x += dx;
		y += dy;
		if (hasborder)
		{
			border(x,y,w,h, s);
		}
		typedef vector<layoutchild> vector_layoutchild; ///me growls at visual studio
		vector_layoutchild::iterator i;
		int c=0;
		for (i = children.begin(); i != children.end(); ++i, c++)
		{
			//draw a border around the object, if it wants us to. 
			if (c == which && focused && (*i).c->highlightEnabled())
			{
				guirect r = (*i).pos;
				r.x += x;
				r.y += y;
				border(r.x,r.y,r.w,r.h, s);
			}
			(*i).c->draw((*i).pos.x + x, (*i).pos.y+y, s);
			
		}
	}

	bool layout::keydown(input::keyboard_event&k)
	{
		if (!visible)
			return false;
		if (children.size())
		{
			//see if the whiched child wants it. 
			if (children[which].c->keydown(k))
				return true;
			//see if the user wants to switch which widget has the which
			switch(k.key())
			{
			case input::keyboard_event::LEFT_KEY:
				if (!moveleft())
					return false;
				LOG(INFO) << logging::indent() << "which is now '" << which << "'";
				break;
			case input::keyboard_event::RIGHT_KEY:
			case input::keyboard_event::TAB_KEY:
				if (!moveright())
					return false;
				LOG(INFO) << logging::indent() << "which is now '" << which << "'";
				break;
			case input::keyboard_event::DOWN_KEY:
				if (!movedown())
					return false;
				LOG(INFO) << logging::indent() << "which is now '" << which << "'";
				break;
			case input::keyboard_event::UP_KEY:
				if (!moveup())
					return false;
				LOG(INFO) << logging::indent() << "which is now '" << which << "'";
				break;
			default:
				return false;
			}
			return true;
		}
		return false;
	}
	bool layout::keyup(input::keyboard_event&k)
	{
		if (!visible)
			return false;
		if (children.size())
		{
			//see if the whiched child wants it. 
			if (children[which].c->keyup(k))
				return true;
		}
		return false;
	}
	bool layout::input(input::keyboard_event&k)
    {
		if (!visible)
			return false;
		if (children.size())
		{
			//see if the whiched child wants it. 
			if (children[which].c->input(k))
				return true;
		}
		return false;
    }
#if 0
	bool layout::mousedown(SDL_MouseButtonEvent &m)
	{
		if (!visible)
			return false;
		if (m.button == SDL_BUTTON_LEFT || m.button == SDL_BUTTON_MIDDLE)
		{
			//search for something that takes the coordinates
			typedef vector<layoutchild> vector_layoutchild;
			vector_layoutchild::iterator i;
			int c=0;
			for (i = children.begin(); i != children.end(); i++,c++)
			{
				//which child did they click on?
				if (m.x > (*i).pos.x && 
					m.y > (*i).pos.y &&
					m.x < (*i).pos.x + (*i).pos.w &&
					m.y < (*i).pos.y + (*i).pos.h)
				{
					bool ret = false;
					Sint16 ox, oy;
					ox = m.x;
					oy = m.y;
					m.x -= (*i).pos.x;
					m.y -= (*i).pos.y;
					if ((*i).c->mousedown(m)) 
					{
						if (which != c)
							children[which].c->unfocus();
						if((ret = (*i).c->focus()))
							which = c;
					}
					m.x = ox;
					m.y = oy;
					mousestate[0] = true;
					return ret;
				}
			}
		}
		return false;
	}
	bool layout::mouseup(SDL_MouseButtonEvent &m)
	{
		if (!visible)
			return false;
		//don't process it unless we saw it go down
		if (m.button == SDL_BUTTON_LEFT && mousestate[0])
		{
			mousestate[0] = false;
			//deliver it to which. 
			bool ret = false;
			Sint16 ox, oy;
			ox = m.x;
			oy = m.y;
			m.x -= children[which].pos.x;
			m.y -= children[which].pos.y;
			children[which].c->mouseup(m);
			m.x = ox;
			m.y = oy;
			return true;
		}
		return false;
	}
#endif
	bool layout::focus()
	{
		if (visible && children.size())
		{
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
				{
					focused = true;
					return true;
				}
				which = (which + 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
		focused = false;
		return false;
	}
	void layout::hide(fadetype f)
	{
		if (visible && fading == NONE)
		{
/*
			switch (f)
			{
			case TOP:
			case BOTTOM:
			case LEFT:
			case RIGHT:
				dx = dy = 0;
				ti = SDL_GetTicks();
				fading = f;
				showing = false;
				break;
			default:
// */
				visible = false;
//			}
		}
	}
	void layout::show(fadetype f)
	{

		if(!visible && fading == NONE)
		{
			visible = true;
/*			switch (f)
			{
			case TOP:
			case BOTTOM:
			case LEFT:
			case RIGHT:
				showing = true;
				dx = dy = 0;
				ti = SDL_GetTicks();
				fading = f;
				break;
			} // */
		}
	}
	void layout::addchild(base& c, int x, int y) 
	{
		guirect p = {x, y, c.getWidth(), c.getHeight()};
		/* if the width is too small, change it to fit */
		if (x + c.getWidth() > w)
			w = x + c.getWidth();
		if (y + c.getHeight() > h)
			h = y + c.getHeight();
		children.push_back(layoutchild(&c, p));
	}
	void layout::removechild(base& c)
	{
		vector<layoutchild>::iterator i;
		for (i = children.begin(); i != children.end(); i++)
		{
			if (&c == (*i).c)
			{
				children.erase(i);
				if (which == i - children.begin())
				{
					//just deleted the selected item.
					//TODO: what happens if nobody takes focus?
					which = 0;
					focus();
				}				
				break;
			}
		}
	}
};
