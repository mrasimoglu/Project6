#ifndef GAMELOOP_H
#define GAMELOOP_H

#pragma once

#include "DXApp.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include "DDSTextureLoader.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "HUD.h"
#include "Map.h"
#include "Camera.h"
#include "Zombie.h"
#include "NavMesh.h"

#include <time.h>
#include <chrono>

using namespace DirectX;
class Item;
class Survivor;
class HUD;


class GameLoop : public DXApp
{
public:
	static std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds> deltaTime;
	static std::chrono::time_point<std::chrono::steady_clock> oldTime;

	GameLoop(HINSTANCE hInstance);
	~GameLoop();
	
	bool Init() override;
	bool InitGame();
	void Update(float dt) override;
	void Render(float dt) override;

private:

	clock_t timer;
	clock_t clockk;
	int counter;
	bool isPressedESC;
	bool isPressedTAB;

	Survivor* survivor;
	HUD* hud;
	Map* map;
	Camera* camera;
	Zombie* zombies[10];
	NavMesh* navMesh;

	std::unique_ptr<Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	Mouse::ButtonStateTracker tracker;
};

#endif