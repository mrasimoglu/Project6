#include "Map.h"
#include <math.h>

void Map::CreateBullet(Vector2 pos, float rot, ID3D11Device* device, Camera* cam, int ammoID)
{
	int i = 0;
	while (i < MAX_BULLET && bullets[i] != nullptr)
		i++;
	
	bullets[i] = new FiniteObject(L"img/gameobjects/bullet.dds", pos, rot, 3000, 3, cam, rectangle, ammoID);
}

void Map::CreateMuzzle(Vector2 pos, float rot, ID3D11Device* device, Camera* cam)
{
	int i = 0;
	while (i < MAX_NOCOLLISION && nocollisions[i] != nullptr)
		i++;

	nocollisions[i] = new FiniteObject(L"img/gameobjects/muzzle.dds", pos, rot, 50, 0, cam, ghost);
}

void Map::CreateBox(Vector2 pos, float rot, ID3D11Device* device, Camera* cam)
{
	int i = 0;
	while (i < MAX_STATIC && statics[i] != nullptr)
		i++;

	statics[i] = new StaticObject(L"img/map/box.dds", pos, rot, rectangle, cam);
	statics[i]->CalculateVertex();
}

void Map::CreateBarrel(Vector2 pos, float rot, ID3D11Device * device, Camera* cam)
{
	int i = 0;
	while (i < MAX_STATIC && statics[i] != nullptr)
		i++;

	statics[i] = new StaticObject(L"img/map/barrel.dds", pos, rot, circle, cam);
	statics[i]->CalculateVertex();
}

void Map::CreateAmmoClipFull(Vector2 pos, float rot, ID3D11Device * device, Camera* cam, int ID, wchar_t* iconpath)
{
	int i = 0;
	while (i < MAX_GROUNDSTATICOBJECTS && groundStaticObjects[i] != nullptr)
		i++;

	groundStaticObjects[i] = new Item(iconpath, L"img/gameobjects/ammo_clip_full.dds", L"Ammo", ID, 100, 1, pos, rot, rectangle, cam);
	groundStaticObjects[i]->GetSprite()->SetScale(Vector2(0.5f, 0.5f));
	((Item*)groundStaticObjects[i])->GetSSIcon()->SetScale(Vector2(0.5f, 0.5f));
}

void Map::CreateGun(Vector2 pos, float rot, Camera *cam, Guns gun)
{
	int i = 0;
	while (i < MAX_GROUNDSTATICOBJECTS && groundStaticObjects[i] != nullptr)
		i++;

	groundStaticObjects[i] = newGun(gun);
	groundStaticObjects[i]->SetPosition(pos);
	groundStaticObjects[i]->SetRotation(rot);
	groundStaticObjects[i]->GetSprite()->SetScale(Vector2(0.5f, 0.5f));
	groundStaticObjects[i]->SetCamera(cam);
}

void Map::CreateAmmoClipEmpty(Vector2 pos, float rot, ID3D11Device * device, Camera* cam)
{
	int i = 0;
	while (i < MAX_GROUNDFINITEOBJECTS && groundFiniteObjects[i] != nullptr)
		i++;

	nocollisions[i] = new FiniteObject(L"img/gameobjects/ammo_clip_empty.dds", pos, rot, 5000, 0, cam, ghost);
	nocollisions[i]->GetSprite()->SetScale(Vector2(0.5f, 0.5f));
}

void Map::CreateBlood(Vector2 pos, float rot, Camera* cam)
{
	int i = 0;
	while (i < MAX_GROUNDFINITEOBJECTS && groundFiniteObjects[i] != nullptr)
		i++;

	nocollisions[i] = new FiniteObject(L"img/map/blood-3.dds", pos, rot, 5000, 0, cam, ghost);
	nocollisions[i]->GetSprite()->SetScale(Vector2(0.1f, 0.1f));
}

void Map::AddItem(Item *item)
{
	int i = 0;
	while (i < MAX_GROUNDSTATICOBJECTS && groundStaticObjects[i] != nullptr)
		i++;

	groundStaticObjects[i] = item;
}

void Map::RemoveItem(Item* item)
{
	int j = 0;
	while (item != groundStaticObjects[j])
		j++;

	groundStaticObjects[j] = nullptr;
}

void Map::DrawMap(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	for (int i = 0; i < MAX_FINITE; i++)
		if (finites[i] != nullptr)
			finites[i]->Draw(spriteBatch);

	for (int i = 0; i < MAX_BULLET; i++)
		if (bullets[i] != nullptr)
			bullets[i]->Draw(spriteBatch);

	for (int i = 0; i < MAX_STATIC; i++)
		if (statics[i] != nullptr)
			statics[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_NOCOLLISION; i++)
		if (nocollisions[i] != nullptr)
			nocollisions[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		if (groundFiniteObjects[i] != nullptr)
			groundFiniteObjects[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_GROUNDSTATICOBJECTS; i++)
		if (groundStaticObjects[i] != nullptr)
			groundStaticObjects[i]->Draw(DXApp::spriteBatch);
}

void Map::DrawFinites()
{
	for (int i = 0; i < MAX_FINITE; i++)
		if (finites[i] != nullptr)
			finites[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_BULLET; i++)
		if (bullets[i] != nullptr)
			bullets[i]->Draw(DXApp::spriteBatch);
}

void Map::DrawStatics()
{
	for (int i = 0; i < MAX_STATIC; i++)
		if (statics[i] != nullptr)
			//if(Collision::Trace(sur, statics, statics[i]->GetPosition(), cam))
				statics[i]->Draw(DXApp::spriteBatch);
}

void Map::DrawGroundObjects()
{
	for (int i = 0; i < MAX_NOCOLLISION; i++)
		if (nocollisions[i] != nullptr)
			nocollisions[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		if (groundFiniteObjects[i] != nullptr)
			groundFiniteObjects[i]->Draw(DXApp::spriteBatch);

	for (int i = 0; i < MAX_GROUNDSTATICOBJECTS; i++)
		if (groundStaticObjects[i] != nullptr)
			groundStaticObjects[i]->Draw(DXApp::spriteBatch);
}

void Map::UpdateMap(DirectX::Mouse::State state)
{
	for (int i = 0; i < MAX_FINITE; i++)
		if (finites[i] != nullptr && finites[i]->Update(this) == 1)
			Memory::SafeDelete(finites[i]);
	for (int i = 0; i < MAX_NOCOLLISION; i++)
		if (nocollisions[i] != nullptr && nocollisions[i]->Update(this) == 1)
			Memory::SafeDelete(nocollisions[i]);
	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		if (groundFiniteObjects[i] != nullptr && groundFiniteObjects[i]->Update(this) == 1)
			Memory::SafeDelete(groundFiniteObjects[i]);

	for (int i = 0; i < MAX_BULLET; i++)
		if (bullets[i] != nullptr)	
		{
			if (bullets[i]->Update(this) == 1)
				Memory::SafeDelete(bullets[i]);

			//BULLETS - STATIC COLLISION
			for (int j = 0; j < MAX_STATIC; j++)
			{
				if (statics[j] != nullptr && bullets[i] != nullptr)
				{
					bullets[i]->CalculateVertex();
					if (Collision::CheckCollision(bullets[i], statics[j]))
					{
						Memory::SafeDelete(bullets[i]);
					}
				}
			}
		}
			
}

void Map::InitMap(ID3D11Device* device, Camera* cam)
{
	this->device = device;

	CreateBarrel(Vector2(300, 300), 0.0f, device, cam);

	CreateBox(Vector2(600, 300), 0, device, cam);
	CreateBox(Vector2(600, 450), 0, device, cam);
	CreateBox(Vector2(600, 600), 0, device, cam);
	CreateBox(Vector2(600, 750), 0, device, cam);
	CreateBox(Vector2(600, 900), 0, device, cam);

	CreateBox(Vector2(750, 300), 0, device, cam);
	CreateBox(Vector2(900, 300), 0, device, cam);
	CreateBox(Vector2(1050, 300), 0, device, cam);
	CreateBox(Vector2(1200, 300), 0, device, cam);
	CreateBox(Vector2(1350, 300), 0, device, cam);
	
	CreateAmmoClipFull(Vector2(300, 100), 0, device, cam, 10, L"img/hud/shotgun_ammo_icon.dds");
	CreateAmmoClipFull(Vector2(300, 100), 0, device, cam, 9, L"img/hud/pistol_ammo_icon.dds");
	CreateAmmoClipFull(Vector2(300, 100), 0, device, cam, 762, L"img/hud/rifle_ammo_icon.dds");
	CreateGun(Vector2(500, 500), 0, cam, pistol);
	CreateGun(Vector2(400, 400), 0, cam, rifle);
}

StaticObject** Map::GetStaticObjects(){	return statics; }

StaticObject** Map::GetGroundObjects(){	return groundStaticObjects; }

int Map::GetMaxStatic(){ return MAX_STATIC; }

int Map::GetMaxGroundObject(){ return MAX_GROUNDSTATICOBJECTS; }

FiniteObject ** Map::GetBullets()
{
	return bullets;
}

int Map::GetMaxBullet()
{
	return MAX_BULLET;
}

Vector2 Map::GetSize()
{
	return this->size;
}

Map::Map(Vector2 size)
{
	this->size = size;

	for (int i = 0; i < MAX_BULLET; i++)
		bullets[i] = nullptr;
	for (int i = 0; i < MAX_STATIC; i++)
		statics[i] = nullptr;
	for (int i = 0; i < MAX_NOCOLLISION; i++)
		nocollisions[i] = nullptr;
	for (int i = 0; i < MAX_FINITE; i++)
		finites[i] = nullptr;
	for (int i = 0; i < MAX_GROUNDSTATICOBJECTS; i++)
		groundStaticObjects[i] = nullptr;
	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		groundFiniteObjects[i] = nullptr;
}

Map::~Map()
{
}
