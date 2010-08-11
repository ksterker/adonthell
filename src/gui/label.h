#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "gui/widget.h"
#include "gui/textcache.h"
#include "gui/font.h"

namespace gui
{
	class label:public widget 
	{
	public:
		label(const u_int16 & width, const u_int16 & height):widget(width,height),CenterX(false),CenterY(false),Multiline(false) 
        { 
            Cache = new text_cache();
            Font = new font(); 
        }
        
		label(const std::string & name):widget(name),CenterX(false),CenterY(false),Multiline(false) 
        {
            Cache = new text_cache();
            Font = new font(); 
        }
        
        virtual ~label()
        {
            delete Cache;
            delete Font;
        }
        
		void set_string(const string& s) 
        { 
            txt = s; 
            Cache->invalidate(); 
        }
        
		const string& get_string() const 
        { 
            return txt; 
        }
        
        font *get_font () const { return Font; }

        /** 
         * Draw the object on the %screen.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL,         
         * draw on the screen.
         */		
        virtual void draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da = NULL, gfx::surface *target = NULL) const;
        
		virtual void set_size(const u_int16 & width, const u_int16 & height) 
        { 
            widget::set_size(width, height);
            Cache->clear ();
        }
        
		void reheight();
		
        void set_center (const bool & cx, const bool & cy ) 
        {
            CenterX = cx;
            CenterY = cy; 
            Cache->invalidate (); 
        }
        
        bool center_x() const { return CenterX; }
        bool center_y() const { return CenterY; }
        
		void set_multiline (const bool & m) { Multiline = m; Cache->invalidate (); }
        bool multiline () const { return Multiline; }
        
		void setColor(const u_int32 & c) { Font->setColor(c); Cache->invalidate (); }
		u_int32 getColor() const { return Font->getColor(); }
        
    protected:
        /// cache of the rendered label text
        text_cache *Cache;
        /// whether to center the text horizontally
        bool CenterX;
        /// whether to center the text vertically
        bool CenterY;
        /// whether the text should wrap across multiple lines
        bool Multiline;
        /// the text
        std::string txt;
        /// the font
		font *Font;
	};
};


#endif//GUI_LABEL_H
