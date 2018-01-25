#pragma once

#include "DXApp.h"
#include <d3d11.h>
#include "SpriteBatch.h"
#include "SimpleMath.h"
#include "DDSTextureLoader.h"
#include "SpriteFont.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Survivor.h"
#include "HUD.h"
#include "Map.h"
#include "Camera.h"

#include <time.h>
#include <chrono>

using namespace DirectX;

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
	SpriteBatch* spriteBatch;
	SpriteFont* spriteFont;

	clock_t timer;
	bool isPressedESC;
	bool isPressedTAB;

	Survivor* survivor;
	HUD* hud;
	Map* map;
	Camera* camera;

	std::unique_ptr<Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	Mouse::ButtonStateTracker tracker;
};
