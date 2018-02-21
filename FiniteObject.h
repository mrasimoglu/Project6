
#ifndef FINITEOBJECT_H
#define FINITEOBJECT_H
#pragma once

#include "Entity.h"
#include "DXUtil.h"

#include "Mustafa.h"

#include <time.h>

class Map;

class FiniteObject : public Entity
{
private:
	clock_t time;
	int lifetime;
	float speed;
	int damage;
	int ammoID;
	int counter;

public:
	bool Update(Map* map);
	void CalculateVertex();
	float GetDamage();

	FiniteObject(wchar_t *path, Vector2 pos, float rot, int lifetime, float speed, Camera*, Shape);
	FiniteObject(wchar_t *path, Vector2 pos, float rot, int lifetime, float speed, Camera*, Shape, int ammoID);
	~FiniteObject();
};

#endif