#include <iostream>
#include "gfx/gfx.h"

int main(int argc, char * argv[])
{
	gfx::init("sdlgl");
	
	gfx::screen::set_video_mode(640, 480, 24);
	
	std::cout << "Oki doki!\n";
	return 0;
}
