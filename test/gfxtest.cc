#include <iostream>
#include "gfx/gfx.h"
#include "base/timer.h"

int main(int argc, char * argv[])
{
	gfx::init(argv[1]);
	std::cout << "1\n";
	gfx::screen::set_video_mode(640, 480, 24);
	std::cout << "2\n";
	gfx::screen::get_surface()->fillrect(0,0,100,100,0xffffff);
	std::cout << "3\n";
	gfx::surface * s = gfx::create_surface();
	s->load_png(argv[2]);
	s->draw(300,50);
	std::cout << "4\n";
	gfx::screen::update();
	base::timer::sleep(1000);
	return 0;
}
