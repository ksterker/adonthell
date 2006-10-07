#include <iostream>
#include "gfx/gfx.h"
#include "base/timer.h"

int main(int argc, char * argv[])
{
	gfx::init(argv[1]);
	gfx::screen::set_video_mode(640, 480, 24);
	gfx::screen::get_surface()->fillrect(0,0,100,100,0x0000ff);
	gfx::surface * s = gfx::create_surface();
	s->load_png(argv[2]);
	s->set_mask(true);
	s->set_alpha(128);
	for (int i = 0; i < 100; i ++)
	{
		for (int j = 0; j < 1000; j++)
			s->draw(rand() % 640,rand() % 480);
		gfx::screen::update();
	}
	return 0;
}
