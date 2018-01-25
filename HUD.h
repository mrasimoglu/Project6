#pragma once


#include "SpriteBatch.h"
#include "SpriteSheet.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Survivor.h"
#include "Button.h"

typedef enum OpenedWindow {mainmenu, loading, menu, game, inventory};

class HUD
{
private:
	Survivor* player;

	OpenedWindow window;

	SpriteSheet* healthBar_green;
	SpriteSheet* healthBar_red;

	SpriteSheet* cursor;

	SpriteSheet* guns;
	SpriteSheet* guns_pointer;

	SpriteSheet* ammo;
	wchar_t ammo_info[10];

	Button* startButton;

	int frame;
public:
	void Draw(SpriteBatch*, SpriteFont*);
	void Update(DirectX::Mouse::State, DirectX::Keyboard::State);
	void SetSurvivor(Survivor*);

	OpenedWindow GetWindow();
	void SetWindow(OpenedWindow);

	HUD(ID3D11Device*);
	~HUD();
};

