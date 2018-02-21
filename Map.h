#define MAX_GROUNDSTATICOBJECTS 100

#define newPistol  new Gun(nullptr, 12, 0, 500, Vector2(87, 20), pistol, L"img/hud/pistol-icon.dds", L"img/hud/pistol.dds", L"Pistol", 11, 1, 1, 9)
#define newShotgun  new Gun(nullptr, 7, 0, 1500, Vector2(115, 16), shotgun, L"img/hud/shotgun-icon.dds", L"img/hud/shotgun.dds", L"Shotgun", 22, 1, 1, 10, true)
#define newRifle  new Gun(nullptr, 30, 0, 200, Vector2(115, 16), rifle, L"img/hud/rifle-icon.dds", L"img/hud/rifle.dds",L"Rifle", 42, 1, 1, 762)
#define newFlashlight  new Gun(nullptr, 1, 0, 0, Vector2(0, 0), flashlight, L"img/hud/flashlight-icon.dds", L"img/hud/flashlight.dds",L"Flashlight", 66, 1, 1, 1)
#define newKnife  new Gun(nullptr, 0, 0, 0, Vector2(0, 0), knife, L"img/hud/knife-icon.dds", L"img/hud/knife.dds",L"Knife", 55, 1, 1, 0)
#define newGun(i) (i==pistol) ?(newPistol):((i==shotgun) ? newShotgun : newRifle)

#ifndef MAP_H
#define MAP_H
#pragma once

#include <d3d11.h>
#include "Mouse.h"

#include "FiniteObject.h"
#include "StaticObject.h"
#include "Camera.h"
#include "Item.h"
#include "Gun.h"
#include "Survivor.h"

#include "DXUtil.h"

#define MAX_STATIC 100
#define MAX_BULLET 100
#define MAX_FINITE 100
#define MAX_NOCOLLISION 100
#define MAX_GROUNDSTATICOBJECTS 100
#define MAX_GROUNDFINITEOBJECTS 100

class Gun;
class Survivor;
class FiniteObject;

class Map
{
private:
	Vector2 size;

	FiniteObject *bullets[MAX_BULLET];
	FiniteObject *finites[MAX_FINITE];
	StaticObject *statics[MAX_STATIC];
	StaticObject *groundStaticObjects[MAX_GROUNDSTATICOBJECTS];
	FiniteObject *groundFiniteObjects[MAX_GROUNDFINITEOBJECTS];
	FiniteObject *nocollisions[MAX_NOCOLLISION];

	ID3D11Device *device;
public:
	void CreateBullet(Vector2 pos, float rot, ID3D11Device* device, Camera*, int ammoID);
	void CreateMuzzle(Vector2 pos, float rot, ID3D11Device* device, Camera*);

	void CreateBox(Vector2 pos, float rot, ID3D11Device* device, Camera*);
	void CreateBarrel(Vector2 pos, float rot, ID3D11Device* device, Camera*);
	
	void CreateAmmoClipFull(Vector2 pos, float rot, ID3D11Device* device, Camera*, int ID, wchar_t*);
	void CreateGun(Vector2 pos, float rot, Camera*, Guns);
	
	void CreateAmmoClipEmpty(Vector2 pos, float rot, ID3D11Device* device, Camera*);
	void CreateBlood(Vector2 pos, float rot, Camera* cam);


	void AddItem(Item*);
	void RemoveItem(Item*);

	void DrawMap(SpriteBatch*, SpriteFont*);
	void DrawFinites();
	void DrawStatics();
	void DrawGroundObjects();
	void UpdateMap(DirectX::Mouse::State);

	void InitMap(ID3D11Device*, Camera*);

	StaticObject** GetStaticObjects();
	int GetMaxStatic();

	StaticObject** GetGroundObjects();
	int GetMaxGroundObject();

	FiniteObject** GetBullets();
	int GetMaxBullet();

	Vector2 GetSize();

	Map(Vector2);
	~Map();
};


#endif