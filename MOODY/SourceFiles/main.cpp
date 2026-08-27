#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::unique_ptr<WE::Framework> game = std::make_unique<MyGame>(L"MOODY");
	game->Run();
	return 0;
}