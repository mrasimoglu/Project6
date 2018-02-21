#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include "Mouse.h"

#include "SpriteSheet.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Button
{
private:
	SpriteSheet* sprite;
	wchar_t* text;

	Vector2 position;
	float rotation;
public:
	Button(ID3D11Device* device, wchar_t *path, wchar_t* text, Vector2 pos, float rot);
	~Button();

	void Draw(SpriteBatch* spriteBatch, SpriteFont*);
	bool isPressed(DirectX::Mouse::State state);
	bool isOver(DirectX::Mouse::State state);
	void Maximize();
	void Minimize();
};

