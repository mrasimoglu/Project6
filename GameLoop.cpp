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
	clockk = clock();
	isPressedESC = false;
	isPressedTAB = false;

	srand(time(NULL));

	if (!DXApp::Init())
		return false;

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
	map->InitMap(m_pDevice, camera);

	navMesh = new NavMesh(map->GetStaticObjects());

	for (int i = 0; i < 1; i++)
	{
		zombies[i] = new Zombie(DXApp::m_pDevice, camera, Vector2(/*rand() % m_ClientWidth, rand() % m_ClientHeight*/1000, 600));
	}

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

	DXApp::mouse = m_mouse->GetState();
	tracker.Update(DXApp::mouse);

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

		survivor->Update(DXApp::mouse, kb, map, zombies);
		map->UpdateMap(DXApp::mouse);
												
		for (int i = 0; i < 10; i++)
			if (zombies[i] != nullptr)
				zombies[i]->Update(map, survivor, zombies);
	}

	hud->Update(DXApp::mouse, kb, map);
}

void GameLoop::Render(float dt)
{
	if (clockk + 1000 < clock())
	{
		printf("FPS : %d\n", counter);
		clockk = clock();
		counter = 0;
	}

	m_pImmidiateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::DimGray);

	DXApp::spriteBatch->Begin();

	if (hud->GetWindow() == game || hud->GetWindow() == inventory)
	{
		map->DrawGroundObjects();

		survivor->Draw(DXApp::spriteBatch);

		for (int i = 0; i < 10; i++)
		{
			if (zombies[i] != nullptr)
				if (Collision::Trace66(zombies[i], survivor->GetPosition(), map->GetStaticObjects(), camera))
					zombies[i]->Draw(DXApp::spriteBatch);
		}

		map->DrawFinites();
	}

	DXApp::spriteBatch->End();

	CommonStates states(m_pDevice);
	m_pImmidiateContext->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_pImmidiateContext->OMSetDepthStencilState(states.DepthNone(), 0);
	m_pImmidiateContext->RSSetState(states.CullNone());
	basicEffect->Apply(m_pImmidiateContext);
	m_pImmidiateContext->IASetInputLayout(inputLayout.Get());
	primitiveBatch->Begin();

	/*auto kb = m_keyboard->GetState();*/

	if ((hud->GetWindow() == game || hud->GetWindow() == inventory)/* && kb.T*/)
	{
		Collision::DrawRay(primitiveBatch, map->GetStaticObjects(), Vector2(survivor->GetPosition()), camera);
	}

	m_pImmidiateContext->Flush();
	primitiveBatch->End();

	//
	DXApp::spriteBatch->Begin();

	if (hud->GetWindow() == game || hud->GetWindow() == inventory)
	{
		map->DrawStatics();
		navMesh->DrawNodes();
	}

	hud->Draw(DXApp::spriteBatch, DXApp::spriteFont);

	DXApp::spriteBatch->End();


	m_pImmidiateContext->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_pImmidiateContext->OMSetDepthStencilState(states.DepthNone(), 0);
	m_pImmidiateContext->RSSetState(states.CullNone());
	basicEffect->Apply(m_pImmidiateContext);
	m_pImmidiateContext->IASetInputLayout(inputLayout.Get());
	primitiveBatch->Begin();

	if (hud->GetWindow() == game)
	{
		navMesh->DrawLinks(primitiveBatch);
	}

	m_pImmidiateContext->Flush();
	primitiveBatch->End();

	HR(m_pSwapChain->Present(0, 0));
	counter++;
}