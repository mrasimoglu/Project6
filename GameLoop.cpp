#include "GameLoop.h"

#include <iostream>
#include <math.h>
#include <conio.h>

#define PI 3.14159265

HFONT Font;

std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds>  GameLoop::deltaTime;
std::chrono::time_point<std::chrono::steady_clock>  GameLoop::oldTime;

GameLoop::GameLoop(HINSTANCE hInstance) : DXApp(hInstance)
{
	spriteBatch = nullptr;
	spriteFont = nullptr;
}

bool GameLoop::Init()
{
	isPressedESC = false;
	isPressedTAB = false;

	srand(time(NULL));

	if (!DXApp::Init())
		return false;

	//Craete spritebatch
	spriteBatch = new SpriteBatch(m_pImmidiateContext);

	//Create spritefont
	spriteFont = new SpriteFont(m_pDevice, L"fonts/Verdana.spriteFont");

	//Mouse and keyboard
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(m_hAppWnd);

	ShowCursor(true);

	hud = new HUD(m_pDevice);

	return true;
}

bool GameLoop::InitGame()
{
	camera = new Camera({ 800, 450 }, { (float)m_ClientWidth, (float)m_ClientHeight });

	survivor = new Survivor(m_pDevice, camera);

	hud->SetSurvivor(survivor);

	map = new Map({2000, 1000});
	map->InitMap(m_pDevice);

	hud->SetWindow(game);

	return true;
}

GameLoop::~GameLoop()
{
	
}

void GameLoop::Update(float dt)
{
	GameLoop::deltaTime = std::chrono::high_resolution_clock::now() - GameLoop::oldTime;
	GameLoop::oldTime = std::chrono::high_resolution_clock::now();

	DirectX::Mouse::State state = m_mouse->GetState();
	tracker.Update(state);

	auto kb = m_keyboard->GetState();

	//
	if (kb.Escape && !isPressedESC)
	{
		if (hud->GetWindow() == menu || hud->GetWindow() == inventory)
			hud->SetWindow(game);
		else if (hud->GetWindow() == game)
			hud->SetWindow(menu);

		isPressedESC = true;
	}
	else if (kb.Tab && !isPressedTAB)
	{
		if (hud->GetWindow() == inventory)
			hud->SetWindow(game);
		else if (hud->GetWindow() == game)
			hud->SetWindow(inventory);
		isPressedTAB = true;
	}

	isPressedESC = kb.Escape;
	isPressedTAB = kb.Tab;

	hud->Update(state, kb);

	if (hud->GetWindow() == loading)
	{
		InitGame();
	}
	if (hud->GetWindow() == game)
	{
		if ((survivor->GetPosition().x > camera->GetView().x / 2) && (survivor->GetPosition().x < map->GetSize().x - camera->GetView().x / 2))
			camera->SetPosition({ survivor->GetPosition().x, camera->GetPosition().y });
		if ((survivor->GetPosition().y > camera->GetView().y / 2) && (survivor->GetPosition().y < map->GetSize().y - camera->GetView().y / 2))
			camera->SetPosition({ camera->GetPosition().x ,survivor->GetPosition().y });

		survivor->Update(state, kb, map);
		map->UpdateMap(state);
	}
}

void GameLoop::Render(float dt)
{
	m_pImmidiateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::DimGray);

	spriteBatch->Begin();

	if (hud->GetWindow() == game || hud->GetWindow() == inventory)
	{
		map->DrawMap(spriteBatch, spriteFont, camera);

		survivor->Draw(spriteBatch, camera);
	}

	hud->Draw(spriteBatch, spriteFont);

	spriteBatch->End();

	HR(m_pSwapChain->Present(0, 0));
}
