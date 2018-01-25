#include "Map.h"

#include <math.h>

#include "Mustafa.h"

void Map::CreateBullet(Vector2 pos, float rot, ID3D11Device* device)
{
	int i = 0;
	while (i < MAX_BULLET && bullets[i] != nullptr)
		i++;
	
	bullets[i] = new FiniteObject(device, L"img/gameobjects/bullet.dds", pos, rot, 3000, 3);
}

void Map::CreateMuzzle(Vector2 pos, float rot, ID3D11Device* device)
{
	int i = 0;
	while (i < MAX_NOCOLLISION && nocollisions[i] != nullptr)
		i++;

	nocollisions[i] = new FiniteObject(device, L"img/gameobjects/muzzle.dds", pos, rot, 50, 0);
}

void Map::CreateBox(Vector2 pos, float rot, ID3D11Device* device)
{
	int i = 0;
	while (i < MAX_STATIC && statics[i] != nullptr)
		i++;

	statics[i] = new StaticObject(device, L"img/map/box.dds", pos, rot, rectangle);
}

void Map::CreateBarrel(Vector2 pos, float rot, ID3D11Device * device)
{
	int i = 0;
	while (i < MAX_STATIC && statics[i] != nullptr)
		i++;

	statics[i] = new StaticObject(device, L"img/map/barrel.dds", pos, rot, circle);
	statics[i]->CalculateVertex();
}

void Map::CreateAmmoClipFull(Vector2 pos, float rot, ID3D11Device * device)
{
	int i = 0;
	while (i < MAX_GROUNDSTATICOBJECTS && groundStaticObjects[i] != nullptr)
		i++;

	groundStaticObjects[i] = new StaticObject(device, L"img/gameobjects/ammo_clip_full.dds", pos, rot, rectangle);
	groundStaticObjects[i]->GetSprite()->SetScale(Vector2(0.5f, 0.5f));
}

void Map::CreateAmmoClipEmpty(Vector2 pos, float rot, ID3D11Device * device)
{
	int i = 0;
	while (i < MAX_GROUNDFINITEOBJECTS && groundFiniteObjects[i] != nullptr)
		i++;

	groundFiniteObjects[i] = new FiniteObject(device, L"img/gameobjects/ammo_clip_empty.dds", pos, rot, 5000, 0);
	groundFiniteObjects[i]->GetSprite()->SetScale(Vector2(0.5f, 0.5f));
}

void Map::DrawMap(SpriteBatch* spriteBatch, SpriteFont* spriteFont, Camera* cam)
{
	for (int i = 0; i < MAX_STATIC; i++)
		if (statics[i] != nullptr)
			statics[i]->Draw(spriteBatch, cam);

	for (int i = 0; i < MAX_FINITE; i++)
		if (finites[i] != nullptr)
			finites[i]->Draw(spriteBatch, cam);

	for (int i = 0; i < MAX_NOCOLLISION; i++)
		if (nocollisions[i] != nullptr)
			nocollisions[i]->Draw(spriteBatch, cam);

	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		if (groundFiniteObjects[i] != nullptr)
			groundFiniteObjects[i]->Draw(spriteBatch, cam);

	for (int i = 0; i < MAX_GROUNDSTATICOBJECTS; i++)
		if (groundStaticObjects[i] != nullptr)
			groundStaticObjects[i]->Draw(spriteBatch, cam);

	for (int i = 0; i < MAX_BULLET; i++)
		if (bullets[i] != nullptr)
			bullets[i]->Draw(spriteBatch, cam);
}

void Map::UpdateMap(DirectX::Mouse::State state)
{
	for (int i = 0; i < MAX_FINITE; i++)
		if (finites[i] != nullptr && finites[i]->Update() == 1)
			Memory::SafeDelete(finites[i]);
	for (int i = 0; i < MAX_NOCOLLISION; i++)
		if (nocollisions[i] != nullptr && nocollisions[i]->Update() == 1)
			Memory::SafeDelete(nocollisions[i]);
	for (int i = 0; i < MAX_GROUNDFINITEOBJECTS; i++)
		if (groundFiniteObjects[i] != nullptr && groundFiniteObjects[i]->Update() == 1)
			Memory::SafeDelete(groundFiniteObjects[i]);

	for (int i = 0; i < MAX_BULLET; i++)
		if (bullets[i] != nullptr)	
		{
			if (bullets[i]->Update() == 1)
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

void Map::InitMap(ID3D11Device* device)
{
	this->device = device;

	CreateBarrel(Vector2(550, 250), 0.0f, device);
	CreateBox(Vector2(850, 600), 0, device);
	CreateAmmoClipFull(Vector2(100, 100), 0, device);
}

StaticObject** Map::GetStaticObjects(){	return statics; }

StaticObject** Map::GetGroundObjects(){	return groundStaticObjects; }

int Map::GetMaxStatic(){ return MAX_STATIC; }

int Map::GetMaxGroundObject(){ return MAX_GROUNDSTATICOBJECTS; }

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
