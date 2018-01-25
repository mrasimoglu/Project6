#include "DXApp.h"
#include "DXUtil.h"
#include "GameLoop.h"

int main(int argc, char** argv)
{
	HINSTANCE hInstance = nullptr;
	GameLoop gameLoop(hInstance);

	if (!gameLoop.Init()) return 1;

	return gameLoop.Run();
}