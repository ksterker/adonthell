#include <iostream>
#include "gfx/gfx.h"
#include "base/timer.h"

#include "main/adonthell.h"

//class AdonthellApp : public adonthell::app
//{

int main(int argc, char * argv[])
//int main()
{
	gfx::init("sdlgl");
	gfx::screen::set_video_mode(640, 480);
	gfx::screen::get_surface()->fillrect(0,0,100,100,0x0000ff);
	gfx::surface * s = gfx::create_surface();
	s->load_png("crono.png");
	s->set_mask(true);
	s->set_alpha(128);
/*	for (int i = 0; i < 100; i ++)
	{
		for (int j = 0; j < 1000; j++)
			s->draw(rand() % 640,rand() % 480);
		gfx::screen::update();
	}*/
	s->draw(10, 10);
	gfx::screen::update();
	sleep(2);
	return 0;
}
//} theApp;
