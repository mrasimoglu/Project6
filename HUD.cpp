#include "HUD.h"

HUD::HUD(ID3D11Device* device)
{
	canPress = true;
	prevTime = clock();

	holdedItem = nullptr;

	invCol = 4;
	invRow = 3;

	inventoryBg = new SpriteSheet(Vector2(800, 450));
	inventoryBg->Load(device, L"img/hud/inventoryBG.dds");
	inventoryBg->SetScale(Vector2(1, 1));

	itemBox = new SpriteSheet();
	itemBox->Load(DXApp::m_pDevice, L"img/hud/itembox.dds");
	itemBox->SetScale(Vector2(1, 1));

	startButton = new Button(device, L"img/hud/startButton.dds", L"Play", Vector2(800,200), 0);

	menuScript = new SpriteSheet(Vector2(800, 100));
	menuScript->Load(device, L"img/hud/menu.dds");
	menuScript->SetScale(Vector2(1, 1));

	inventoryScript = new SpriteSheet(Vector2(800, 100));
	inventoryScript->Load(device, L"img/hud/inventory.dds");
	inventoryScript->SetScale(Vector2(1, 1));

	frame = 0;

	window = mainmenu;

	cursor = new SpriteSheet();
	cursor->Load(device, L"img/hud/cursor2.dds");
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

	guns[0] = new SpriteSheet();
	guns[0]->SetScale(Vector2(1, 1));
	guns[0]->SetPosition(Vector2(200, 702));
	guns[0]->Load(device, L"img/HUD/guns.dds");

	guns[1] = new SpriteSheet();
	guns[1]->SetScale(Vector2(1, 1));
	guns[1]->SetPosition(Vector2(200, 750));
	guns[1]->Load(device, L"img/HUD/pistol.dds");
	guns[2] = new SpriteSheet();
	guns[2]->SetScale(Vector2(1, 1));
	guns[2]->SetPosition(Vector2(200, 782));
	guns[2]->Load(device, L"img/HUD/pistol.dds");
	guns[3] = new SpriteSheet();
	guns[3]->SetScale(Vector2(1, 1));
	guns[3]->SetPosition(Vector2(200, 814));
	guns[3]->Load(device, L"img/HUD/pistol.dds");

	guns_pointer = new SpriteSheet();
	guns_pointer->SetScale(Vector2(1, 1));
	guns_pointer->SetPosition(Vector2(200, 750));
	guns_pointer->Load(device, L"img/HUD/guns_pointer.dds");

	ammo = new SpriteSheet();
	ammo->SetScale({ 1,1 });
	ammo->SetOrigin({ 0, 0 });
	ammo->SetPosition({ 350, 800 });
}

void HUD::Update(DirectX::Mouse::State mouse, DirectX::Keyboard::State keyboard, Map* map)
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

		guns_pointer->SetPosition(Vector2(200, 686 + player->GetOnHand() * 32));
		
		for (int i = 1; i < 4; i++)
		{
			Guns temp = player->GetOnHandType(i + 1);
			if (temp == pistol)
				guns[i]->Load(DXApp::m_pDevice, L"img/hud/pistol.dds");
			else if (temp == rifle)
				guns[i]->Load(DXApp::m_pDevice, L"img/hud/rifle.dds");
			else if (temp == shotgun)
				guns[i]->Load(DXApp::m_pDevice, L"img/hud/shotgun.dds");
			else if (temp = noGun)
				guns[i]->Load(DXApp::m_pDevice, L"img/hud/noGun.dds");
		}

		swprintf(ammo_info, sizeof(wchar_t) * 10, L"%d%d/%d%d", player->GetAmmoCurrent() / 10, player->GetAmmoCurrent() % 10, player->GetAmmoBackup() / 10, player->GetAmmoBackup() % 10);

		cursor->SetPosition(Vector2(mouse.x, mouse.y));
	}break;
	case menu:
	{

	}break;
	case inventory:
	{
		if (holdedItem != nullptr)
			holdedItem->GetSSIcon()->SetPosition(Vector2((float)DXApp::mouse.x, (float)DXApp::mouse.y));
		

		if (holdedItem != nullptr && !mouse.leftButton)//Býrakýldýðý an
		{
			if(player->GetInventory()->GetPlaceFromItem(holdedItem) != -1) // ÝNVENTORYDEN ITEM
			// inventorydeki býrakýldýgý an 
			{
				int top = inventoryBg->GetPosition().y - inventoryBg->GetHeight() / 2 + 190, left = inventoryBg->GetPosition().x - inventoryBg->GetWidth() / 2 + 102;

				for (int i = 0; i < invCol * invRow; i++)
					if (Collision::isOver(mouse, { (float)(left + 180 * (i % invCol)), (float)(top + 167 * (i / (invRow + 1))) }, Vector2(90, 84)))
						player->GetInventory()->replaceItem(holdedItem, i);

				if(holdedItem->IsHandable())
					for (int i = 2; i < MAX_WEAPON; i++)
							if (Collision::isOver(mouse, { (float)1400, (float)(200 + (100 * (i))) }, Vector2(90, 84)))
								player->SwitchInventoryOnHand(i, player->GetInventory()->GetPlaceFromItem(holdedItem));

				if (mouse.x < 400)
					player->DropItem(holdedItem, map);
			}
			else if(player->GetPlaceFromItem(holdedItem) != -1)
			// ELÝMÝZDEKI BIRAKILDI an
			{
				for (int i = 2; i < MAX_WEAPON; i++)
					if (Collision::isOver(mouse, { (float)1400, (float)(200 + (100 * (i))) }, Vector2(90, 84)))
						player->SwitchOnHand(i, player->GetPlaceFromItem(holdedItem));
				int top = inventoryBg->GetPosition().y - inventoryBg->GetHeight() / 2 + 190, left = inventoryBg->GetPosition().x - inventoryBg->GetWidth() / 2 + 102;

				for (int i = 0; i < invCol * invRow; i++)
					if (Collision::isOver(mouse, { (float)(left + 180 * (i % invCol)), (float)(top + 167 * (i / (invRow + 1))) }, Vector2(90, 84)))
						if (player->GetInventory()->GetItemFromPlace(i) != nullptr ? player->GetInventory()->GetItemFromPlace(i)->IsHandable() : true)
						{
							if (player->GetPlaceFromItem(holdedItem) == player->GetOnHand())
								player->SetOnHand(knife);
							player->SwitchOnHandInventory(player->GetPlaceFromItem(holdedItem), i);
						}
			}
			else
			//yakýndakilerin býrakýldýðý an
			{
				if (mouse.x > 400)
					player->TakeItem(holdedItem, map);
			}
			holdedItem = nullptr;

		}

		//inventory sað click
		Item** items = player->GetInventory()->GetItems();
		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (items[i] != nullptr)
			{
				SpriteSheet* s = items[i]->GetSSIcon();
				if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)) && DXApp::mouse.rightButton && canPress)
				{
					player->DropItem(items[i],map);
				}
			}
		}

		//yakýndakiler sað click
		Item** cItems = (Item**)player->GetClosestGroundObjects();
		int i = 0;
		while (cItems[i] != nullptr)
		{
			SpriteSheet* s = cItems[i]->GetSSIcon();
			if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)) && DXApp::mouse.rightButton && canPress)
			{
				player->TakeItem(cItems[i],map);
				cItems[i] = nullptr;
			}
			i++;
		}

		//elindekiler sað click
		Gun** onHands = player->GetOnHands();
		for (int i = 2; i < MAX_WEAPON; i++)
		{
			if (onHands[i] != nullptr)
			{
				SpriteSheet* s = onHands[i]->GetSSIcon();
				if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)) && DXApp::mouse.rightButton && canPress)
				{
					player->DropItemToInventory(onHands[i]);
				}
			}
		}

		//Ele alma
		items = player->GetInventory()->GetItems();
		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (items[i] != nullptr)
			{
				SpriteSheet* s = items[i]->GetSSIcon();
				if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)) && DXApp::mouse.leftButton  && holdedItem == nullptr)
				{
					holdedItem = items[i];
				}
			}
		}

		Gun** onHand = player->GetOnHands();
		for (int i = 2; i < MAX_WEAPON; i++)
			if (Collision::isOver(mouse, { (float)1400, (float)(200 + (100 * (i))) }, Vector2(90, 84)) && mouse.leftButton && holdedItem == nullptr)
				holdedItem = onHand[i];

		cItems = (Item**)player->GetClosestGroundObjects();
		i = 0;
		while (cItems[i] != nullptr)
		{
			SpriteSheet* s = cItems[i]->GetSSIcon();
			if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)) && DXApp::mouse.leftButton && holdedItem == nullptr)
			{
				holdedItem = cItems[i];
				break;
			}
			i++;
		}
	}break;
	}

	if (prevTime + 100 < clock())
		canPress = true;

	if (mouse.rightButton)
	{
		canPress = false;
		prevTime = clock();
	}
}

void HUD::DrawInventory(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	int top = inventoryBg->GetPosition().y-inventoryBg->GetHeight()/2 + 190, left= inventoryBg->GetPosition().x - inventoryBg->GetWidth() / 2 + 102;
	inventoryBg->Draw(spriteBatch);
	Item** items = player->GetInventory()->GetItems();
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (items[i] != nullptr && holdedItem != items[i])
		{
			SpriteSheet* s = items[i]->GetIcon();
			if (Collision::isOver(DXApp::mouse, s->GetPosition(), Vector2(s->GetWidth() * s->GetScale().x, s->GetHeight() * s->GetScale().y)))
			{																					
				items[i]->GetInfoBox()->Draw(DXApp::mouse);
			}

			items[i]->GetSSIcon()->SetPosition({(float)(left + 180 * (i % invCol )), (float)(top + 167 * (i / (invRow+1)))});
			items[i]->GetSSIcon()->Draw(spriteBatch);

			wchar_t text[5];

			swprintf(text, sizeof(wchar_t) * 5, L"%d", items[i]->GetNum());

			spriteFont->DrawString(spriteBatch, text, Vector2(left + 180 * (i % invCol) + 44, top + 167 * (i / (invRow + 1)) + 40), Colors::Blue, 0.f, Vector2(0, 0), Vector2(0.75f, 0.75f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
		}
	}

	Item** cItems = (Item**)player->GetClosestGroundObjects();
	int i = 0;
	while (cItems[i] != nullptr)
	{
		itemBox->SetPosition(Vector2(200, 200 + 100 * (i + 1)));
		itemBox->Draw(spriteBatch);
		cItems[i]->GetSSIcon()->SetPosition(Vector2(200, 200 + 100 * (i+1)));
		cItems[i]->GetSSIcon()->Draw();

		i++;
	}

	Gun** onHands = player->GetOnHands();
	for (int i = 0; i < MAX_WEAPON; i++)
	{
		itemBox->SetPosition(Vector2(1400, 200 + (100 * i)));
		itemBox->Draw();
		if (onHands[i] != nullptr  && holdedItem != onHands[i])
		{
			onHands[i]->GetSSIcon()->SetPosition((Vector2(1400, 200 + (100 * i))));
			onHands[i]->GetSSIcon()->Draw();
		}
	}

	if(holdedItem != nullptr)
		holdedItem->GetIcon()->Draw(DXApp::spriteBatch);
}

void HUD::Draw(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	switch (window)
	{
	case mainmenu:
	{
		startButton->Draw(spriteBatch, spriteFont);
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

		for (int i = 0; i < 4; i++)
			guns[i]->Draw(spriteBatch);

		guns_pointer->Draw(spriteBatch);

		ammo->DrawString(spriteFont, spriteBatch, ammo_info);

		if (player->GetClosestGroundObjects()[0] != nullptr)
		{
			spriteFont->DrawString(spriteBatch, L"Press 'E' for interaction", player->GetClosestGroundObjects()[0]->GetPosition(), Colors::White.v, 0, Vector2(100, 0), Vector2(0.5f, 0.5f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
		}
			

		cursor->Draw(spriteBatch);
	}break;
	case menu:
	{
		menuScript->Draw(spriteBatch);
	}break;
	case inventory:
	{
		inventoryScript->Draw(spriteBatch);
		DrawInventory(spriteBatch, spriteFont);
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

