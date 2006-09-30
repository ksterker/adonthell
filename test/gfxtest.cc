#include <iostream>
#include "gfx/gfx.h"

int main(int argc, char * argv[])
{
	gfx::init(argv[1]);
	std::cout << "1\n";
	gfx::screen::set_video_mode(640, 480, 24);
	std::cout << "2\n";
	gfx::screen::get_surface()->fillrect(0,0,100,100,0xffffff);
	std::cout << "3\n";
	gfx::screen::update();
	std::cout << "4\n";
	sleep(1);
	return 0;
}
