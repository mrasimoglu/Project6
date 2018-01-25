#pragma once

#include "Gun.h"
#include "Character.h"
#include "CharacterAnimations.h"

#include <d3d11.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Map.h"

#define MAX_WEAPON 5

typedef enum Guns { flashlight, knife, pistol, shotgun, rifle };

class Gun;

class Survivor : public Character
{
private:
	ID3D11Device* device;

	Camera* camera;

	Guns gun;
	Gun* guns[MAX_WEAPON];

	CharacterAnimations* anim;

	int scrollStore; //scroll'la silah deðiþtirmek için
	StaticObject* closestGroundObjects[MAX_GROUNDSTATICOBJECTS/3];

	BodyAction FeetActionToBodyAction(FeetAction);
	void CalculateVertex();
public:
	Guns GetOnHand();
	int GetAmmoCurrent();
	int GetAmmoBackup();
	Camera* GetCamera();
	StaticObject** GetClosestGroundObjects();

	Survivor(ID3D11Device*, Camera*);
	float CalculateRotation(DirectX::Mouse::State state, float rot, Vector2 position);
	void Update(DirectX::Mouse::State, DirectX::Keyboard::State, Map*);

	~Survivor();
};

