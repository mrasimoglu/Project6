#ifndef HUD_H
#define HUD_H
#pragma once


#include "SpriteBatch.h"
#include "SpriteSheet.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Survivor.h"
#include "Button.h"

enum OpenedWindow {mainmenu, loading, menu, game, inventory};

class Survivor;

class HUD
{
private:
	Item* holdedItem;

	Survivor* player;

	SpriteSheet* menuScript;
	SpriteSheet* inventoryScript;

	SpriteSheet* inventoryBg;
	SpriteSheet* itemBox;

	int invRow;
	int invCol;

	OpenedWindow window;

	SpriteSheet* healthBar_green;
	SpriteSheet* healthBar_red;

	SpriteSheet* cursor;

	SpriteSheet* guns[4];
	SpriteSheet* guns_pointer;

	SpriteSheet* ammo;
	wchar_t ammo_info[10];

	
	clock_t prevTime;
	bool canPress;

	Button* startButton;

	int frame;

	void DrawInventory(SpriteBatch* spriteBatch, SpriteFont* spriteFont);
		
public:
	void Draw(SpriteBatch*, SpriteFont*);
	void Update(DirectX::Mouse::State, DirectX::Keyboard::State, Map*);
	void SetSurvivor(Survivor*);

	OpenedWindow GetWindow();
	void SetWindow(OpenedWindow);

	HUD(ID3D11Device*);
	~HUD();
};

#endif