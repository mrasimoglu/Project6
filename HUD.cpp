#include "HUD.h"

#include "Mouse.h"
#include "Keyboard.h"

HUD::HUD(ID3D11Device* device)
{
	startButton = new Button(device, L"img/hud/startButton.dds", Vector2(800,200), 0);

	frame = 0;

	window = mainmenu;

	cursor = new SpriteSheet();
	cursor->Load(device, L"img/hud/cursor.dds");
	cursor->SetScale(Vector2(0.25f, 0.25f));

	healthBar_green = new SpriteSheet();
	healthBar_green->SetScale(Vector2(2.5f, 1));
	healthBar_green->SetPosition(Vector2(1250, 800));
	healthBar_green->Load(device, L"img/HUD/healthbar_green.dds");
	healthBar_green->SetOrigin(Vector2(0, 0));

	healthBar_red = new SpriteSheet();
	healthBar_red->SetScale(Vector2(2.5f, 1));
	healthBar_red->SetPosition(Vector2(1250, 800));
	healthBar_red->Load(device, L"img/HUD/healthbar_red.dds");
	healthBar_red->SetOrigin(Vector2(0, 0));

	guns = new SpriteSheet();
	guns->SetScale(Vector2(1, 1));
	guns->SetPosition(Vector2(200, 750));
	guns->Load(device, L"img/HUD/guns.dds");

	guns_pointer = new SpriteSheet();
	guns_pointer->SetScale(Vector2(1, 1));
	guns_pointer->SetPosition(Vector2(200, 750));
	guns_pointer->Load(device, L"img/HUD/guns_pointer.dds");

	ammo = new SpriteSheet();
	ammo->SetScale({ 1,1 });
	ammo->SetOrigin({ 0, 0 });
	ammo->SetPosition({ 350, 800 });
}

void HUD::Update(DirectX::Mouse::State mouse, DirectX::Keyboard::State keyboard)
{
	switch (window)
	{
	case mainmenu:
	{
		if (startButton->isOver(mouse))
		{
			startButton->Maximize();
			if (mouse.leftButton)
				window = loading;
		}
		else
			startButton->Minimize();
	
	}break;
	case loading:
	{

	}break;
	case game:
	{
		healthBar_green->SetScale(Vector2(player->GetHealth()*0.025f, 1));

		guns_pointer->SetPosition(Vector2(200, + 686 + player->GetOnHand() * 32));

		swprintf(ammo_info, sizeof(wchar_t) * 10, L"%d%d/%d%d", player->GetAmmoCurrent() / 10, player->GetAmmoCurrent() % 10, player->GetAmmoBackup() / 10, player->GetAmmoBackup() % 10);

		cursor->SetPosition(Vector2(mouse.x, mouse.y));
	}break;
	case menu:
	{

	}break;
	case inventory:
	{

	}break;
	}
}

void HUD::Draw(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	switch (window)
	{
	case mainmenu:
	{
		startButton->Draw(spriteBatch);
	}break;
	case loading:
	{
		spriteFont->DrawString(spriteBatch, L"Loading", Vector2(800, 450), Colors::White.v, 0, Vector2(45, 0), Vector2(2.0f, 2.0f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
		for(int i=0; i<(frame/30)%4; i++)
			spriteFont->DrawString(spriteBatch, L".", Vector2(895, 450), Colors::White.v, 0, Vector2(0 - i*5, 0), Vector2(2.0f, 2.0f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
	}break;
	case game:
	{
		healthBar_red->Draw(spriteBatch);
		healthBar_green->Draw(spriteBatch);

		guns->Draw(spriteBatch);
		guns_pointer->Draw(spriteBatch);

		ammo->DrawString(spriteFont, spriteBatch, ammo_info);

		if (player->GetClosestGroundObjects()[0] != nullptr)
			spriteFont->DrawString(spriteBatch, L"Press 'E' for interaction", player->GetClosestGroundObjects()[0]->GetPosition(), Colors::White.v, 0, Vector2(100, 0), Vector2(0.5f, 0.5f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);

		cursor->Draw(spriteBatch);
	}break;
	case menu:
	{

	}break;
	case inventory:
	{

	}break;
	}

	frame++;
}

void HUD::SetSurvivor(Survivor *survivor)
{
	player = survivor;
}

OpenedWindow HUD::GetWindow()
{
	return window;
}

void HUD::SetWindow(OpenedWindow window)
{
	this->window = window;

	if (window == game)
		ShowCursor(false);
	else
		ShowCursor(true);
}


HUD::~HUD()
{
}
