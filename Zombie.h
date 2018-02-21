#ifndef ZOMBIE_H
#define ZOMBIE_H

#pragma once

#include "StaticObject.h"
#include "Mustafa.h"

#include "Character.h"
#include "Map.h"
#include "Camera.h"

enum zombieAction {idleZ, moveZ, attackZ, deadZ};

class Map;
class Survivor;

class Zombie : public Character
{
private:
	SpriteSheet* sprites[3];
	int frame;

	Vector2 target;
	clock_t lastSeen;

	zombieAction action;

public:
	Zombie(ID3D11Device*, Camera*, Vector2);
	void Update(Map*, Survivor*, Zombie**);
	void IncFrame();
	void CalculateVertex();
	~Zombie();
};

#endif