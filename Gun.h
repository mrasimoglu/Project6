#ifndef GUN_H
#define GUN_H

#pragma once

#include <time.h>

#include <d3d11.h>
#include "SimpleMath.h"



#include "Map.h"
#include "Survivor.h"
#include "Camera.h"
#include "Item.h"
#include "DXApp.h"

using namespace DirectX::SimpleMath;

class Item;
enum Guns;
class Survivor;
class Map;

class Gun : public Item
{
private:
	Survivor* owner;

	Guns type;

	int AmmoID;
	int maxAmmo;
	int backup;
	int currentAmmo;
	
	int delay;
	int rebound;

	bool singleReload;
	
	clock_t time;

	Vector2 offset;
public:
	bool Shoot(int, Map*, Vector2, Camera*);
	bool CanReload();
	int Reload();
	void Fire(Map*, Vector2, Camera*);

	int GetBackup();
	int GetCurrentAmmo();
	int GetMaxAmmo();
	int GetRebound();
	Guns GetType();
	int GetAmmoID();

	void SetRebound(int);
	void SetBackup(int);
	void SetOwner(Survivor*);
	void Update();

	Vector2 CalculateRelativePosition();
	float CalculateRelativeRotation(Vector2);
	Gun(Survivor*, int, int, int, Vector2, Guns, wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, int AmmoID);
	Gun(Survivor*, int, int, int, Vector2, Guns, wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, int AmmoID, bool);
	Gun(Survivor*, int, int, int, Vector2, Guns,int);
	Gun(Survivor*, int, int, int, Vector2, bool, Guns, int);
	~Gun();
};

#endif