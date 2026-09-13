#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WE::uPtr<WE::Framework> game = std::make_unique<MyGame>(L"MOODY");
	game->Run();
	return 0;
}