#pragma once

#include "DXApp.h"

#include <d3d11.h>
#include "Mouse.h"
#include "SpriteSheet.h"

class GameLoop;

class InfoBox
{
private:
	wchar_t* info;
	Vector2 twh;
	SpriteSheet* sprite;
public:
	InfoBox(wchar_t*);
	~InfoBox();

	void Draw(DirectX::Mouse::State);
};

