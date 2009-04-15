#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "widget.h"
#include "font.h"

namespace gui
{
	class label:public widget 
	{
	protected:
		string txt;
		font f;
		gfx::surface * cached;
		bool cachevalid;
		int offset;
		bool centerx, centery;
		int px, py;
		int rx, ry; //the offset where the text was actually rendered
		bool _multiline;
	public:
		label(int width, int height):widget(width,height),cached(NULL),cachevalid(false),offset(0),centerx(false),centery(false),px(0),py(0),_multiline(false) {}
		label(const char* bgfile):widget(bgfile),cached(NULL),cachevalid(false),offset(0),centerx(false),centery(false),px(0),py(0),_multiline(false) {}
		~label() {if (cached) delete cached;}

		void setString(const string& s) { txt = s; cachevalid = false; }
		const string& getString() { return txt; }
		void setFont(const font& newfont) {f = newfont; cachevalid = false; }

		virtual void draw(int x, int y, gfx::surface* s);

		virtual void setSize(int width, int height) {w = width; h = height; delete cached; cached = NULL; cachevalid = false;}
		void reheight();
		void centerH(bool c) {centerx = c; cachevalid=false; }
		void centerV(bool c) {centery = c; cachevalid=false; }
		void multiline(bool m) {_multiline = m; cachevalid=false;}
		void setColor(unsigned int c) {f.setColor(c); cachevalid=false;}
		unsigned int getColor() {return f.getColor();}
	};
};


#endif//GUI_LABEL_H
