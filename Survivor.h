#ifndef SURVIVOR_H
#define SURVIVOR_H

#pragma once

#include "Character.h"
#include "CharacterAnimations.h"

#include <d3d11.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Map.h"
#include "Zombie.h"

#include "Gun.h"

#include "Inventory.h"

enum BodyAction { idle, move, reload, shoot, meleeattack };
enum FeetAction { idleF, walkF, runF, leftF, rightF };

#define MAX_WEAPON 5

enum Guns { flashlight, knife, pistol, shotgun, rifle, noGun };

class Item;
class Gun;
class Map;
class Zombie;

class Survivor : public Character
{
private:
	ID3D11Device* device;

	Inventory* inventory;

	BodyAction bodyAction;
	FeetAction feetAction;
	
	Guns gun;
	Gun* onHand[MAX_WEAPON];

	CharacterAnimations* anim;

	int scrollStore; //scroll'la silah deðiþtirmek için
	StaticObject* closestGroundObjects[MAX_GROUNDSTATICOBJECTS/3];

	BodyAction FeetActionToBodyAction(FeetAction);
	void CalculateVertex();

	void UpdateGunsBackup();

	int bodyFrame;
	int feetFrame;
	void IncFrame();
public:
	void TakeItem(Item *,Map*);
	void DropItem(Item *, Map*);
	void DropItemToInventory(Item *);

	Inventory* GetInventory();
	Guns GetOnHand();
	Gun** GetOnHands();
	int GetAmmoCurrent();
	int GetAmmoBackup();
	Camera* GetCamera();
	Guns GetOnHandType(int);
	StaticObject** GetClosestGroundObjects();

	int GetPlaceFromItem(Item*);

	void SwitchInventoryOnHand(int, int);
	void SwitchOnHand(int, int);
	void SwitchOnHandInventory(int, int);

	void SetOnHand(Guns);

	Survivor(ID3D11Device*, Camera*);
	float CalculateRotation(DirectX::Mouse::State state, float rot, Vector2 position);
	void Update(DirectX::Mouse::State, DirectX::Keyboard::State, Map*, Zombie* zombies[10]);

	~Survivor();
};

#endif