
#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gfx/gfx.h"
#include <string>
using std::string;
#include <vector>
#ifndef SWIG
using std::vector;
#endif

namespace gui
{
	class textsize
	{
	public:
		int w, h;
		int cpos;
		textsize(int _w, int _h, int c): w(_w),h(_h),cpos(c) {}
	};
	class font
	{
	private:
		void ref(bool addref);
		FT_Face face;
		bool valid;
		int fontsize;
		int error;
		void draw_glyph(int x, int y, FT_Bitmap* bmp, gfx::surface* s);
		u_int32 color;
	public:
		font(const char* path=NULL, int size=24);
		~font();

		void render(const string& s, int x, int y, gfx::surface* surf);
		void render(const char* s, int x, int y, gfx::surface* surf) {render(string(s),x,y,surf);}
		
		bool isValid() { return !error; }
		int getError() { return error; }
		void setSize(int size); 
		int getSize() { return fontsize; }
		void getSize(const string& s, int& w, int & h);
		void getSize(const char* s, int&w, int&h) {getSize(string(s), w, h);}
	//	void getMultilineSize(const char* s, int maxwidth, vector<int>& vpos, int & w, int & h)
	//	{getMultilineSize(string(s), maxwidth, vpos, w, h);}
		void getMultilineSize(const string& s, int maxwidth, vector<textsize>& ts, int & w, int & h);
		u_int32 getColor() {return color;}
		void setColor(u_int32 c) {color = c;}
	};
};

#endif//FONT_H

